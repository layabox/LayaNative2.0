/****************************************************************************
 Copyright (c) 2010-2014 laya8.net
 Copyright (c) 2014 Kuo.wang

"[WebSocket module] is based in part on the work of the libwebsockets  project
(http://libwebsockets.org)"

 ****************************************************************************/

#ifndef __LAYA_WEBSOCKET_H__
#define __LAYA_WEBSOCKET_H__

#include <vector>
#include <list>
#include <string>

struct lws;
struct lws_context;
struct lws_protocols;
struct lws_vhost;
struct lws_context_creation_info;

namespace laya{

#define LAYA_SAFE_DELETE(p)            do { if(p) { delete (p); (p) = 0; } } while(0)
#define LAYA_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = 0; } } while(0)
#define LAYA_SAFE_FREE(p)                do { if(p) { free(p); (p) = 0; } } while(0)
#define LAYA_SAFE_RELEASE(p)            do { if(p) { (p)->release(); } } while(0)
#define LAYA_SAFE_RELEASE_NULL(p)        do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define LAYA_SAFE_RETAIN(p)            do { if(p) { (p)->retain(); } } while(0)
#define LAYA_BREAK_IF(cond)            if(cond) break

class WsThreadHelper;
class WsMessage;



class WebSocket
{
public:
    static std::string s_strProxy;
	WebSocket();
	virtual ~WebSocket();
    
	/**
		*  @brief Data structure for message
		*/
	struct Data
	{
		Data():bytes(NULL), len(0), isBinary(false){}

		bool isBinary;
		char* getPayload() {
			return bytes + bytesWritten;
		}
		void updateBytesWritten(uint32_t bytes) {
			bytesWritten += bytes;
		}
		uint32_t getBytesLeft() {
			return len - bytesWritten;
		}
		int bytesWritten = 0;
		char* bytes;
		uint32_t len;
	};
    
	/**
		*  @brief Errors in websocket
		*/
	enum class ErrorCode
	{
		TIME_OUT,
		CONNECTION_FAILURE,
		UNKNOWN,
	};
    
	/**
		*  Websocket state
		*/
	enum class State
	{
		CONNECTING,
		OPEN,
		CLOSING,
		CLOSED,
	};

	/**
		*  @brief The delegate class to process websocket events.
		*/
	class Delegate
	{
	public:
		virtual ~Delegate() {}
		virtual void onOpen(WebSocket* ws) = 0;
		virtual void onMessage(WebSocket* ws, const Data& data) = 0;
		virtual void onClose(WebSocket* ws) = 0;
		virtual void onError(WebSocket* ws, const ErrorCode& error) = 0;
	};
    
    
	/**
		*  @brief  The initialized method for websocket.
		*          It needs to be invoked right after websocket instance is allocated.
		*  @param  delegate The delegate which want to receive event from websocket.
		*  @param  url      The URL of websocket server.
		*  @return true: Success, false: Failure
		*/
	bool init(const Delegate& delegate,
				const std::string& url,
				const std::vector<std::string>* protocols = NULL);
    
	void clearDelegate(){
		m_delegate = NULL;
	}
	/**
		*  @brief Sends string data to websocket server.
		*/
	void send(const std::string& message);
    
	/**
		*  @brief Sends binary data to websocket server.
		*/
	void send(const unsigned char* binaryMsg, unsigned int len);
    
	/**
		*  @brief Closes the connection to server.
		*/
	void close();
    
	/**
		*  @brief Gets current state of connection.
		*/
	State getReadyState();
    
private:
	virtual void onSubThreadStarted();
	virtual int onSubThreadLoop();
	virtual void onSubThreadEnded();
	virtual void onJSThreadReceiveMessage(WsMessage* msg);

    lws_vhost* createVhost(struct lws_protocols* protocols, int& sslConnection);
    lws_context_creation_info createContextCreationInfo(const struct lws_protocols* protocols, bool peerServerCert);

	friend class WebSocketCallbackWrapper;
	int onSocketCallback(
							struct lws *wsi,
							int reason,
							void *user, void *in, size_t len);
    
private:
	State        m_readyState;
	bool		m_bWantClose;
	std::string  m_host;
	unsigned int m_port;
	std::string  m_path;
    std::string  m_origin;
	friend class WsThreadHelper;
	WsThreadHelper* m_wsHelper;
    
	struct lws*         m_wsInstance;
	struct lws_context* m_wsContext;
	Delegate* m_delegate;
	int m_SSLConnection;
	struct lws_protocols* m_wsProtocols;

    bool m_bLastIsBin = false;
    char* m_pLastData = nullptr;
    size_t m_nCurDataLen = 0;
	std::string m_supportedProtocols;
};
}

#endif /* defined(__LAYA_WEBSOCKET_H__) */
