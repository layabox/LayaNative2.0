/****************************************************************************
 Copyright (c) 2014 kuo.wang
 
"[WebSocket module] is based in part on the work of the libwebsockets  project
(http://libwebsockets.org)"

 ****************************************************************************/

#include "WebSocket.h"
#include "Uri.h"
#include <thread>
#include <mutex>
#include <queue>
#include <signal.h>
#include <errno.h>
#include "../JCConch.h"
#include <libwebsockets.h>
#include <thread>
#include <stdlib.h>
#include <downloadCache/JCFileSource.h>
#include <algorithm>
#ifdef ANDROID
#include <downloadCache/JCAndroidFileSource.h>
#elif __APPLE_
#include <downloadCache/JCIosFileSource.h>
#elif OHOS
#include <downloadCache/JCOHOSFileSource.h>
#endif
#include "util/Log.h"
#ifdef WIN32
#include <filesystem>
namespace  fs = std::experimental::filesystem::v1;
#ifdef min
#undef min
#endif
#else
#include "fileSystem/JCFileSystem.h"
#endif
extern std::string gRedistPath;
extern std::string gResourcePath;
const uint32_t RX_BUFFER_SIZE = 65536;
namespace laya{

    std::string WebSocket::s_strProxy;
class WsMessage
{
public:
	WsMessage() : what(0), obj(NULL){}
	unsigned int what; // message type
	void* obj;
};

/**
 *  @brief Websocket thread helper, it's used for sending message between JS thread and websocket thread.
 */
class WsThreadHelper
{
public:
	WsThreadHelper();
	~WsThreadHelper();
        
	// Creates a new thread
	bool createThread(const WebSocket& ws);
	// Quits sub-thread (websocket thread).
	void quitSubThread();
    
	// Sends message to UI thread. It's needed to be invoked in sub-thread.
	void sendMessageToJSThread(WsMessage *msg);
    
	// Sends message to sub-thread(websocket thread). It's needs to be invoked in JS thread.
	void sendMessageToSubThread(WsMessage *msg);
    
	// Waits the sub-thread (websocket thread) to exit,
	void joinSubThread();
    
protected:
	void wsThreadEntryFunc();
    
private:
	std::list<WsMessage*>* m_subThreadWsMessageQueue;
	//std::mutex   m_subThreadWsMessageQueueMutex;
	//std::thread* _subThreadInstance;

	WebSocket* m_pWebSocket;
	bool m_needQuit;
	std::thread  *m_subThreadInstance;
	std::mutex   *m_subThreadWsMessageQueueMutex;
	std::mutex   *m_JSWsMessageQueueMutex;	

	friend class WebSocket;
};

// Wrapper for converting websocket callback from static function to member function of WebSocket class.
class WebSocketCallbackWrapper {
public:
    
	static int onSocketCallback(
								struct lws *wsi,
								lws_callback_reasons reason,
								void *user, void *in, size_t len)
	{
        if (reason == LWS_CALLBACK_GET_THREAD_ID) {
            //���Ҳ��֪����ʲô�ã�����㷵��һ��ֵ��
            auto id = std::this_thread::get_id();
            std::hash<std::thread::id> sh;
            return sh(id);
        }
		// Gets the user data from context. We know that it's a 'WebSocket' instance.
        struct lws_context* pCtx = lws_get_context(wsi);
        WebSocket* wsInstance = (WebSocket*)(lws_context_user(pCtx));//lws_context_user(ctx);
		if (wsInstance)
		{
			return wsInstance->onSocketCallback(wsi, reason, user, in, len);
		}
		return 0;
	}
};

// Implementation of WsThreadHelper
WsThreadHelper::WsThreadHelper()
: m_subThreadInstance(nullptr)
  ,m_subThreadWsMessageQueueMutex(NULL)
  ,m_pWebSocket(NULL) 
  ,m_needQuit(false)
{
	m_subThreadWsMessageQueue = new std::list<WsMessage*>();
}

WsThreadHelper::~WsThreadHelper()
{
	joinSubThread();
	LAYA_SAFE_DELETE(m_subThreadInstance);
	LAYA_SAFE_DELETE(m_subThreadWsMessageQueueMutex);
	LAYA_SAFE_DELETE(m_JSWsMessageQueueMutex);
	delete m_subThreadWsMessageQueue;
}

bool WsThreadHelper::createThread(const WebSocket& ws)
{
	m_pWebSocket = const_cast<WebSocket*>(&ws);
    
	// Creates websocket thread
	m_subThreadWsMessageQueueMutex = new std::mutex();
	m_JSWsMessageQueueMutex = new std::mutex();
	m_subThreadInstance = new std::thread(&WsThreadHelper::wsThreadEntryFunc,this);
	return true;
}

void WsThreadHelper::quitSubThread()
{
	m_needQuit = true;
}

void WsThreadHelper::wsThreadEntryFunc()
{
	m_pWebSocket->onSubThreadStarted();
	//�ȵȸ�500�����ٵ��� libwebsocket_service 
	//�����ٵڶ��ε���connect��ʱ��������������win64��ws2���첽���ӵ����⣿��
#ifdef WIN32
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
#endif
	while (!m_needQuit)
	{
		if (m_pWebSocket->onSubThreadLoop())
		{
			break;
		}
	}
    
	m_pWebSocket->onSubThreadEnded();
}

void WsThreadHelper::sendMessageToJSThread(WsMessage *msg)
{
	std::lock_guard<std::mutex> lock(*m_JSWsMessageQueueMutex);
	m_pWebSocket->onJSThreadReceiveMessage(msg);
}

void WsThreadHelper::sendMessageToSubThread(WsMessage *msg)
{
    std::lock_guard<std::mutex> lock(*m_subThreadWsMessageQueueMutex);
	m_subThreadWsMessageQueue->push_back(msg);
}

void WsThreadHelper::joinSubThread()
{
	if(m_subThreadInstance->joinable())
	{
		m_subThreadInstance->join();
	}
}

enum WS_MSG {
	WS_MSG_TO_SUBTRHEAD_SENDING_STRING = 0,
	WS_MSG_TO_SUBTRHEAD_SENDING_BINARY,
	WS_MSG_TO_JS_THREAD_OPEN,
	WS_MSG_TO_JS_THREAD_MESSAGE,
	WS_MSG_TO_JS_THREAD_ERROR,
	WS_MSG_TO_JS_THREAD_CLOSE
};

WebSocket::WebSocket()
: m_readyState(State::CONNECTING)
, m_port(80)
, m_wsHelper(nullptr)
, m_wsInstance(nullptr)
, m_wsContext(nullptr)
, m_delegate(nullptr)
, m_SSLConnection(0)
, m_wsProtocols(nullptr)
, m_bWantClose(false)
{
}

WebSocket::~WebSocket()
{
	close();
	LAYA_SAFE_DELETE(m_wsHelper);
    
	for (int i = 0; m_wsProtocols[i].callback != nullptr; ++i)
	{
		LAYA_SAFE_DELETE_ARRAY(m_wsProtocols[i].name);
	}
	LAYA_SAFE_DELETE_ARRAY(m_wsProtocols);
}

bool WebSocket::init(const Delegate& delegate,
						const std::string& url,
						const std::vector<std::string>* protocols/* = NULL*/)
{
	bool ret = false;
    
	m_delegate = const_cast<Delegate*>(&delegate);
    
    Uri uri = Uri::parse(url);


    int port = static_cast<int>(uri.getPort());
    if (port == 0)
        port = uri.isSecure() ? 443 : 80;

    std::string path = uri.getPathEtc();
    if (path.empty())
        path = "/";

    
	m_host = uri.getHostName();
	m_port = port;
	m_path = path;
    m_origin = uri.getAuthority();
	m_SSLConnection = uri.isSecure() ? LCCSCF_USE_SSL : 0;
    
	LOGI("WebSocket::init m_host: %s, m_port: %d, m_path: %s", m_host.c_str(), m_port, m_path.c_str());

	int protocolCount = 0;
	if (protocols && protocols->size() > 0)
	{
		protocolCount = protocols->size();
	}
	else
	{
		protocolCount = 1;
	}
    
	m_wsProtocols = new lws_protocols[protocolCount+1];
	memset(m_wsProtocols, 0, sizeof(lws_protocols)*(protocolCount+1));

	if (protocols)
	{
		int i = 0;
		for (std::vector<std::string>::const_iterator iter = protocols->begin(); iter != protocols->end(); ++iter, ++i)
		{
			char* name = new char[(*iter).length()+1];
			strcpy(name, (*iter).c_str());
			m_wsProtocols[i].name = name;
			m_wsProtocols[i].rx_buffer_size = RX_BUFFER_SIZE;
			m_wsProtocols[i].callback = WebSocketCallbackWrapper::onSocketCallback;
		}
	}
	else
	{
		char* name = new char[20];
		strcpy(name, "default-protocol");
		m_wsProtocols[0].name = name;
		m_wsProtocols[0].rx_buffer_size = RX_BUFFER_SIZE;	//������ﲻ�裬����android�µ����͵����ݺܶ��ʱ�򣬻ᵼ�·���ʧ�ܣ�����0��
												//������ʱ��򲻵��־�����Ϊ�����ץ���������Ƿ��͵�Ϊԭʼ���ݣ�ͬ��ͨsocket��֪��Ϊʲô�������޹أ���
		m_wsProtocols[0].callback = WebSocketCallbackWrapper::onSocketCallback;
	}
    
	// WebSocket thread needs to be invoked at the end of this method.
	m_wsHelper = new WsThreadHelper();
	ret = m_wsHelper->createThread(*this);
    
	return ret;
}

void WebSocket::send(const std::string& message)
{
	if (m_readyState == State::OPEN)
	{
		// In main thread
		WsMessage* msg = new WsMessage();
		msg->what = WS_MSG_TO_SUBTRHEAD_SENDING_STRING;
		Data* data = new Data();
		data->bytes = new char[message.length()+1];
		memcpy(data->bytes, message.c_str(), message.length());
		data->len = message.length();
        data->bytes[message.length()] = '\0';
		msg->obj = data;
		m_wsHelper->sendMessageToSubThread(msg);
	}
}

void WebSocket::send(const unsigned char* binaryMsg, unsigned int len)
{
	//assert(binaryMsg != nullptr && len > 0, "parameter invalid.");

	if (m_readyState == State::OPEN)
	{
		// In main thread
		WsMessage* msg = new WsMessage();
		msg->what = WS_MSG_TO_SUBTRHEAD_SENDING_BINARY;
		Data* data = new Data();
		data->bytes = new char[len];
		memcpy((void*)data->bytes, (void*)binaryMsg, len);
		data->len = len;
		msg->obj = data;
		m_wsHelper->sendMessageToSubThread(msg);
	}
}

void WebSocket::close()
{
	if (m_readyState == State::CLOSING || m_readyState == State::CLOSED)
	{
		return;
	}
    
	LOGI("websocket connection closed by client");
	//TODO ������ʵ��Ӧ������. ԭ���ǿ�������ص�ws������ʵ����
	//������������ױ�ws�߳��޸ģ���������join��ʱ������
	m_readyState = State::CLOSED;	
	
	m_bWantClose = true;
	lws_cancel_service(m_wsContext);
	m_wsHelper->joinSubThread();
    
	// onClose callback needs to be invoked at the end of this method
	// since websocket instance may be deleted in 'onClose'.
	if(m_delegate)
		m_delegate->onClose(this);
}

WebSocket::State WebSocket::getReadyState()
{
	return m_readyState;
}

int WebSocket::onSubThreadLoop()
{
	if (m_readyState == State::CLOSED || m_readyState == State::CLOSING || m_bWantClose)
	{
		lws_context_destroy(m_wsContext);
		// return 1 to exit the loop.
		return 1;
	}
    
	if (m_wsContext && m_readyState != State::CLOSED && m_readyState != State::CLOSING)
	{
		lws_service(m_wsContext, 0);
	}
    
	// Sleep 3 ms
	std::this_thread::sleep_for(std::chrono::milliseconds(3));
	//boost::this_thread::sleep(boost::posix_time::milliseconds(50));

	return 0;
}
	void WebSocket::onSubThreadStarted() {


        //定义重试策略（关闭 LWS 自动 ping/pong）
           lws_retry_bo_t retry_policy = {
               .retry_ms_table = NULL,
               .retry_ms_table_count = 0,
               .jitter_percent = 0,
               .secs_since_valid_ping = 0xffff,
               .secs_since_valid_hangup = 0xffff,
           };

           lws_context_creation_info info = createContextCreationInfo(m_wsProtocols, true);

           info.retry_and_idle_policy = &retry_policy;
           info.timeout_secs = 0xffff;

        
	m_wsContext = lws_create_context(&info);
	if (nullptr != m_wsContext)
	{

		static const struct lws_extension EXTENSIONS[] =
		{
			{"permessage-deflate",
			 lws_extension_callback_pm_deflate,
			 // client_no_context_takeover extension is not supported in the current version, it will cause connection fail
			 // It may be a bug of lib websocket build
			 //            "permessage-deflate; client_no_context_takeover; client_max_window_bits"
			 "permessage-deflate; client_max_window_bits"},
			{"deflate-frame",
			 lws_extension_callback_pm_deflate,
			 "deflate_frame"},
			{nullptr, nullptr, nullptr /* terminator */} };

		m_readyState = State::CONNECTING;

		struct lws_vhost* vhost = nullptr;

		vhost = createVhost(m_wsProtocols, m_SSLConnection);

		if (s_strProxy.length() > 0)
		{
			lws_set_proxy(vhost, s_strProxy.c_str());
		}

		struct lws_client_connect_info connectInfo;
		memset(&connectInfo, 0, sizeof(connectInfo));
		connectInfo.context = m_wsContext;
		connectInfo.address = m_host.c_str();
		connectInfo.port = m_port;
		connectInfo.ssl_connection = m_SSLConnection;
		connectInfo.path = m_path.c_str();
		connectInfo.host = m_host.c_str();
		connectInfo.origin = m_origin.c_str();
		connectInfo.protocol = m_supportedProtocols.empty() ? nullptr : m_supportedProtocols.c_str();
		connectInfo.ietf_version_or_minus_one = -1;
		connectInfo.userdata = this;
		connectInfo.client_exts = EXTENSIONS;
		connectInfo.vhost = vhost;

		m_wsInstance = lws_client_connect_via_info(&connectInfo);

		if (m_wsInstance == nullptr)
		{
			//connection Error();
			return;
		}
	}
}
void WebSocket::onSubThreadEnded()
{

}

int WebSocket::onSocketCallback(
						struct lws *wsi,
						int reason,
						void *user, void *in, size_t len)
{

	//assert(m_wsContext == nullptr || ctx == m_wsContext);
	//assert(m_wsInstance == nullptr || wsi == nullptr || wsi == m_wsInstance);
	switch (reason)
    {
        case LWS_CALLBACK_DEL_POLL_FD:
		case LWS_CALLBACK_PROTOCOL_DESTROY:
		case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
			{
				LOGI("WebSocket onSocketCallback connection error %s %d reason", in != nullptr ? (const char*)in : "", reason);
				WsMessage* msg = nullptr;
				if (reason == LWS_CALLBACK_CLIENT_CONNECTION_ERROR
					|| (reason == LWS_CALLBACK_PROTOCOL_DESTROY && m_readyState == State::CONNECTING)
					|| (reason == LWS_CALLBACK_DEL_POLL_FD && m_readyState == State::CONNECTING)
					)
				{
					msg = new WsMessage();
					msg->what = WS_MSG_TO_JS_THREAD_ERROR;
					m_readyState = State::CLOSING;
				}
				else if (reason == LWS_CALLBACK_PROTOCOL_DESTROY && m_readyState == State::CLOSING)
				{
					msg = new WsMessage();
					msg->what = WS_MSG_TO_JS_THREAD_CLOSE;
				}

				if (msg)
				{
					m_wsHelper->sendMessageToJSThread(msg);
				}
			}
            break;
		case LWS_CALLBACK_CLIENT_ESTABLISHED:
			{
                WsMessage* msg = new WsMessage();
                msg->what = WS_MSG_TO_JS_THREAD_OPEN;
                m_readyState = State::OPEN;
                
                /*
                 * start the ball rolling,
                 * LWS_CALLBACK_CLIENT_WRITEABLE will come next service
                 */
                lws_callback_on_writable( wsi);
                m_wsHelper->sendMessageToJSThread(msg);
			}
			break;
            
		case LWS_CALLBACK_CLIENT_WRITEABLE:
			{
				do {
					std::lock_guard<std::mutex> lock(*m_wsHelper->m_subThreadWsMessageQueueMutex);

					std::list<WsMessage *>::iterator iter = m_wsHelper->m_subThreadWsMessageQueue->begin();

					int bytesWrite = 0;
					for (; iter != m_wsHelper->m_subThreadWsMessageQueue->end();) {
						WsMessage *subThreadMsg = *iter;

						if (WS_MSG_TO_SUBTRHEAD_SENDING_STRING == subThreadMsg->what
							|| WS_MSG_TO_SUBTRHEAD_SENDING_BINARY == subThreadMsg->what) {
							Data *data = (Data *) subThreadMsg->obj;

							uint32_t toWriteSize = std::min(RX_BUFFER_SIZE, data->getBytesLeft());
							unsigned char *buf = new unsigned char[LWS_SEND_BUFFER_PRE_PADDING
																   + toWriteSize];

							memset(&buf[LWS_SEND_BUFFER_PRE_PADDING], 0, toWriteSize);
							memcpy((char *) &buf[LWS_SEND_BUFFER_PRE_PADDING], data->getPayload(),
								   toWriteSize);

							int writeProtocol;
							uint32_t bytesLeft = data->getBytesLeft();
							if (data->bytesWritten == 0) {
								if (WS_MSG_TO_SUBTRHEAD_SENDING_STRING == subThreadMsg->what) {
									writeProtocol = LWS_WRITE_TEXT;
								} else {
									writeProtocol = LWS_WRITE_BINARY;
								}
								if (data->len > RX_BUFFER_SIZE) {
									writeProtocol |= LWS_WRITE_NO_FIN;
								}
							} else {
								writeProtocol = LWS_WRITE_CONTINUATION;
								if (data->getBytesLeft() != toWriteSize) {
									writeProtocol |= LWS_WRITE_NO_FIN;
								}
							}

							bytesWrite = lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING],
												   toWriteSize,
												   static_cast<lws_write_protocol>(writeProtocol));
							/*if (bytesWrite == 0) {
								//��ʱ�޷����ͣ��Ȼ������
								break;
							}
							if (bytesWrite == 0) {
								break;
							}*/
							if (bytesWrite < 0) {
								//���������ˡ�
								LOGE("WebSocket::onSocketCallback libwebsocket_write error! ");
								LAYA_SAFE_DELETE_ARRAY(data->bytes);
								LAYA_SAFE_DELETE(data);
								LAYA_SAFE_DELETE_ARRAY(buf);
								break;
							}
							else if (bytesWrite < toWriteSize) {
								data->updateBytesWritten(bytesWrite);
								LAYA_SAFE_DELETE_ARRAY(buf);
							}
							else if (bytesLeft == bytesWrite) {
								LAYA_SAFE_DELETE_ARRAY(data->bytes);
								LAYA_SAFE_DELETE(data);
								LAYA_SAFE_DELETE_ARRAY(buf);

								iter = m_wsHelper->m_subThreadWsMessageQueue->erase(iter);
								LAYA_SAFE_DELETE(subThreadMsg);
								break;
							}
							else {
								data->updateBytesWritten(bytesWrite);
								LAYA_SAFE_DELETE_ARRAY(buf);
							}
							}
							//if (bytesWrite < data->len) {
							//}


						}



//				m_wsHelper->m_subThreadWsMessageQueue->clear();


				} while(false);

				/* get notified as soon as we can write again */

				lws_callback_on_writable(wsi);
			}
			break;
            
		case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
			{
				// Remote peer sent a Close frame. Mark as closing; libwebsockets
				// will transition to CLOSED and trigger the closed callback below.
				m_readyState = State::CLOSING;
				LOGI("WebSocket onSocketCallback peer initiated close (%zu bytes)", len);
			}
			break;
		case LWS_CALLBACK_WSI_DESTROY:
		case LWS_CALLBACK_CLIENT_CLOSED:
		case LWS_CALLBACK_CLOSED:
			{
                
				// CCLOG("%s", "connection closing..");
				LOGI("WebSocket onSocketCallback connection closing %d reason", reason);
				m_wsHelper->quitSubThread();
                
				if (m_readyState != State::CLOSED)
				{
					WsMessage* msg = new WsMessage();
					m_readyState = State::CLOSED;
					msg->what = WS_MSG_TO_JS_THREAD_CLOSE;
					m_wsHelper->sendMessageToJSThread(msg);
				}
			}
			break;
            
		case LWS_CALLBACK_CLIENT_RECEIVE:
			{
                //bool finalfrg = lws_is_final_fragment(wsi)!=0;   �����Զ����1
                const size_t remaining = lws_remaining_packet_payload(wsi);
				if (in && len > 0){
                    if (remaining>0) {
                        //֡������
                        char* pData = new char[m_nCurDataLen + len];
                        if(m_nCurDataLen)
                            memcpy(pData, m_pLastData, m_nCurDataLen);
                        memcpy(pData + m_nCurDataLen, in, len);

                        if (m_pLastData) {
                            delete [] m_pLastData;
                            if (m_bLastIsBin != lws_frame_is_binary(wsi)) {
                                LOGE("warn:difference frame type��");
                            }
                        }
                        m_pLastData = pData;
                        m_nCurDataLen += len;
                        m_bLastIsBin = lws_frame_is_binary(wsi);
                        //append(in, len);
                    }
                    else {
                        char* pData =(char*) in;
                        size_t nLen = len;
                        bool needDel = false;
                        if (m_pLastData) {
                            needDel = true;
                            pData = new char[m_nCurDataLen+len];
                            memcpy(pData, m_pLastData,m_nCurDataLen);
                            memcpy(pData + m_nCurDataLen, in, len);
                            nLen += m_nCurDataLen;
                            delete[] m_pLastData;
                            m_pLastData = nullptr;
                            m_nCurDataLen = 0;
                        }

                        WsMessage* msg = new WsMessage();
                        msg->what = WS_MSG_TO_JS_THREAD_MESSAGE;

                        char* bytes = NULL;
                        Data* data = new Data();

                        if (lws_frame_is_binary(wsi)) {
                            bytes = new char[nLen];
                            data->isBinary = true;
                        }
                        else {
                            bytes = new char[nLen + 1];
                            bytes[nLen] = '\0';
                            data->isBinary = false;
                        }

                        memcpy(bytes, pData, nLen);
                        data->bytes = bytes;
                        data->len = nLen;
                        msg->obj = (void*)data;

                        if (needDel) {
                            delete[] pData;
                        }

                        m_wsHelper->sendMessageToJSThread(msg);
                    }
				}
			}
			break;
		default:
			break;
        
	}
    
	return 0;
}

void WebSocket::onJSThreadReceiveMessage(WsMessage* msg)
{
	if (!m_delegate)
		return;

	switch (msg->what) {
		case WS_MSG_TO_JS_THREAD_OPEN:
			{
				m_delegate->onOpen(this);
			}
			break;
		case WS_MSG_TO_JS_THREAD_MESSAGE:
			{
                /*
				Data* data = (Data*)msg->obj;
				m_delegate->onMessage(this, *data); //TODO ���ָ��ֱ�Ӵ���js��js��ɾ�����ܱ���һ��new��memcpy
				LAYA_SAFE_DELETE_ARRAY(data->bytes);
				LAYA_SAFE_DELETE(data);
                */
                m_delegate->onMessage(this, *(Data*)msg->obj);
			}
			break;
		case WS_MSG_TO_JS_THREAD_CLOSE:
			{
				m_delegate->onClose(this);
			}
			break;
		case WS_MSG_TO_JS_THREAD_ERROR:
			{
				// FIXME: The exact error needs to be checked.
				WebSocket::ErrorCode err = ErrorCode::CONNECTION_FAILURE;
				m_delegate->onError(this, err);
			}
			break;
		default:
			break;
	}
}

lws_context_creation_info WebSocket::createContextCreationInfo(const struct lws_protocols* protocols, bool peerServerCert)
{
    lws_context_creation_info info;
    memset(&info, 0, sizeof(info));
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;

    //info.extensions = exts;

    info.gid = -1;
    info.uid = -1;
    if (peerServerCert)
    {
        info.options = LWS_SERVER_OPTION_EXPLICIT_VHOSTS | LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
    }
    else
    {
        info.options = LWS_SERVER_OPTION_EXPLICIT_VHOSTS | LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT | LWS_SERVER_OPTION_PEER_CERT_NOT_REQUIRED;
    }
    info.user = this;

    return info;
}

lws_vhost* WebSocket::createVhost(struct lws_protocols* protocols, int& sslConnection)
{
    std::string caFileName = "cacert.pem";
#ifdef WIN32
    static std::string caFilePath = gRedistPath + std::string("ca/") + caFileName;
    bool isCAFileExist = fs::exists(caFilePath);
#else
    static std::string caFilePath = gResourcePath + "ca/" + caFileName;
    bool isCAFileExist = JCConch::s_pAssetsFiles->isFileExist(("ca/" + caFileName).c_str());
#endif

    lws_context_creation_info info = createContextCreationInfo(protocols, isCAFileExist);

    if (sslConnection != 0)
    {
        if (isCAFileExist)
        {
#if defined(ANDROID) || defined(OHOS)
            // if ca file is in the apk, try to extract it to writable path
            std::string writablePath = gRedistPath;
            static std::string newCaFilePath = writablePath + caFileName;

            if (fs::exists(newCaFilePath))
            {
                LOGI("ca file already exists in apk [%s]", newCaFilePath.c_str());
                info.ssl_ca_filepath = newCaFilePath.c_str();
            }
            else
            {
                char* sJSRuntime = NULL;
                int nSize = 0;
                if (JCConch::s_pAssetsFiles->loadFileContent(caFilePath.c_str(), sJSRuntime, nSize))
                {
                    if (writeFileSync1(newCaFilePath.c_str(), sJSRuntime, nSize))
                    {
                        info.ssl_ca_filepath = newCaFilePath.c_str();
                        LOGE("copy ca file succeed");
                    }
                    else
                    {
                        LOGE("write ca file failed while copy it to apk");
                        sslConnection = sslConnection | LCCSCF_ALLOW_SELFSIGNED | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;
                    }
                }
                else
                {
                    LOGE("read ca file failed while copy it to apk");
                    sslConnection = sslConnection | LCCSCF_ALLOW_SELFSIGNED | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;
                }
            }
#else
            LOGI("find ca file in [%s]", caFilePath.c_str());
            info.ssl_ca_filepath = caFilePath.c_str();
#endif
        }
        else
        {
            LOGI("can not find ca file in [%s]", caFilePath.c_str());
            sslConnection = sslConnection | LCCSCF_ALLOW_SELFSIGNED | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;
        }
    }

    lws_vhost* vhost = lws_create_vhost(m_wsContext, &info);

    return vhost;
}
}

