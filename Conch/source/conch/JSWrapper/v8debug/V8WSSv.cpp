#ifdef JS_V8_DEBUGGER
#include "V8WSSv.h"
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <mutex>


namespace laya {

    struct vhdinfo {
        struct lws_context *context;
        struct lws_vhost *vhost;
        int *interrupted;
        int *options;
    };

    /*
    ���� https://github.com/warmcat/libwebsockets/tree/master/minimal-examples/ws-server/minimal-ws-server-echo
    ��Ҫ�л��汾��
    */

    per_session_data__v8dbg* pCurPss = nullptr;
    DebuggerAgent* gpDbgAgent = nullptr;
    static int callback_def(
        struct lws *wsi, enum lws_callback_reasons reason,
        void *user, void *in, size_t len) {

        int n;
        int sendMsgLen = 0;
        per_session_data__v8dbg *pss = (per_session_data__v8dbg *)user;

        switch (reason) {
        case LWS_CALLBACK_PROTOCOL_INIT:
            break;
        case LWS_CALLBACK_ESTABLISHED:  //������������
            printf("connection established\n");
            pss->pRecvBuff = nullptr;
            pss->index = 0;
            pss->nRecvLen = -1;
            pss->pDbgAgent = gpDbgAgent;
            new (&pss->pTaskLock) std::recursive_mutex();
            new (&pss->pSendTask) std::deque<std::string>();
            //new (&pss->pSendTask) std::vector<std::string>();
            gpDbgAgent->onAcceptNewFrontend(pss);
            pCurPss = pss;
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE: //���Է�����
            //ȱʡ���Ǽ������͡�������һ�ο���ֻ������һ����
            n = LWS_WRITE_CONTINUATION;
            if (!pss->continuation) {
                //������µ����񣬾������ı����߶����ơ�
                if (pss->binary)
                    n = LWS_WRITE_BINARY;
                else
                    n = LWS_WRITE_TEXT;
                pss->continuation = 1;
            }
            if (!pss->final)
                n |= LWS_WRITE_NO_FIN;

            //pss->tx += pss->len;
            pss->pTaskLock.lock();
            //һ�δ���һ��
            if (pss->pSendTask.size() > 0) {
                std::string& t1 = pss->pSendTask.front();
                //printf("send:%s\n", t1.substr(0,200).c_str());
                sendMsgLen = t1.length();
                if (pss->pSendBuff) {
                    delete[] pss->pSendBuff;
                }
                pss->pSendBuff = new unsigned char[sendMsgLen + LWS_PRE];
                memcpy(pss->pSendBuff + LWS_PRE, t1.c_str(), sendMsgLen);
                //ɾ����һ��
                pss->pSendTask.pop_front();
            }
            pss->pTaskLock.unlock();
            if (sendMsgLen > 0) {
                n = lws_write(wsi, &pss->pSendBuff[LWS_PRE], sendMsgLen, (lws_write_protocol)n);
                if (n < 0) {
                    lwsl_err("ERROR %d writing to socket, hanging up\n", n);
                    return 1;
                }
                if (n < (int)sendMsgLen) {
                    lwsl_err("Partial write\n");
                    //���ﲻ֪����ô����������˵���ַ��͵�������ùܡ��´�LWS_CALLBACK_SERVER_WRITEABLE��ʱ��Ӧ���Ѿ�ȫ�������ˡ�
                    //����pss->continuationӦ�û���=0
                    //����ֱ�ӷ��أ�����pss->continuation����1�ˣ��ǹ����ô
                    return -1;
                }
            }
            if (pss->final)
                pss->continuation = 0;
            /* ��ᵼ��websocket������д����-1
            //һ��ȫ������
            pss->pTaskLock.lock();
            if (pss->pSendTask.size() > 0) {
                for (std::string& t1 : pss->pSendTask) {
                    printf("send:%s\n", t1.c_str());
                    sendMsgLen = t1.length();
                    if (pss->pSendBuff) {
                        delete[] pss->pSendBuff;//TODO �Ż�
                    }
                    pss->pSendBuff = new unsigned char[sendMsgLen + LWS_PRE];
                    memcpy(pss->pSendBuff + LWS_PRE, t1.c_str(), sendMsgLen);
                    n = lws_write(wsi, &pss->pSendBuff[LWS_PRE], sendMsgLen, (lws_write_protocol)n);
                    if (n < 0) {
                        lwsl_err("ERROR %d writing to socket, hanging up\n", n);
                        return 1;
                    }
                    if (n < (int)sendMsgLen) {
                        lwsl_err("Partial write\n");
                        //���ﲻ֪����ô����������˵���ַ��͵�������ùܡ��´�LWS_CALLBACK_SERVER_WRITEABLE��ʱ��Ӧ���Ѿ�ȫ�������ˡ�
                        //����pss->continuationӦ�û���=0
                        //����ֱ�ӷ��أ�����pss->continuation����1�ˣ��ǹ����ô
                        return -1;
                    }
                }
            }
            pss->pTaskLock.unlock();
            */
            //lws_rx_flow_control(wsi, 1);    //�����ڴ������Ͻ������ݡ�
            lws_callback_on_writable(wsi);//һ���л��ᣬ����д�ص�
            break;
        case LWS_CALLBACK_RECEIVE: { // ���Խ����ˡ�
                                     // Create a buffer to hold our response
                                     // it has to have some pre and post padding.
                                     // You don't need to care what comes there, libwebsockets
                                     // will do everything for you. For more info see
                                     // http://git.warmcat.com/cgi-bin/cgit/libwebsockets/tree/lib/libwebsockets.h#n597

            //amsg.first = lws_is_first_fragment(wsi);
            pss->final = lws_is_final_fragment(wsi);    //�Ƿ������һ����Ϣ��
            pss->binary = lws_frame_is_binary(wsi);     //�Ƿ��Ƕ����Ƶġ�
            const size_t remaining = lws_remaining_packet_payload(wsi);
            //lwsl_info("+++ test-echo: RX len %ld final %ld, pss->len=%ld\n",(long)len, (long)pss->final, (long)pss->len);

            if (in && len > 0) {
                if (remaining > 0) {
                    //֡�������� TODO
                    *(int*)0 = 1;
                }
                else {
                    if (len+LWS_PRE > sizeof(pss->RecvBuf)) {
                        *(int*)0 = 1;//TODO 
                    }
                    memcpy(&pss->RecvBuf[LWS_PRE], in, len);
                    pss->RecvBuf[LWS_PRE + len] = 0;
                    pss->nRecvLen = (unsigned int)len;
                    pss->rx += len;
                    pss->pDbgAgent->onDbgMsg((char*)(pss->RecvBuf + LWS_PRE), len);
                }
            }

            //lws_rx_flow_control(wsi, 0);//��ֹ�ڴ������Ͻ������ݡ�
            lws_callback_on_writable(wsi);//һ���л��ᣬ����д�ص�
            break;
        }

        case LWS_CALLBACK_CLOSED:
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            lwsl_debug("closed\n");
            pCurPss = nullptr;
            if (gpDbgAgent) {
                gpDbgAgent->onFrontEndClose();
            }
            //state = 0;
            break;

        default:
            break;
        }

        return 0;
    }

#define RING_DEPTH 4096

    static struct lws_protocols protocols[] = {
        /* first protocol must always be HTTP handler */
        {
            "http-only",   // name
            callback_def, // callback
            sizeof(per_session_data__v8dbg),              // per_session_data_size
            MAX_V8DBG_PAYLOAD
        },
        {
            NULL, NULL, 0   /* End of list */
        }
    };

    std::thread* wssvth = nullptr;
    bool interrupted = false;

    void wsserver_run(lws_context* context) {
        int n = 0;
        while (n >= 0 && !interrupted) {
            //nWSSVSleep �� timeout_ms: �ȴ���ʱʱ�䣬��û���ҵ���Ҫ������������Ҫ�ȴ���ʱ�䣬Ϊ0���������أ�
            //�����һͨ���ص�������Ϣ��
            int nSleep = 10;
            if (pCurPss) {
                pCurPss->pTaskLock.lock();
                if (pCurPss->pSendTask.size() > 0) {
                    nSleep = 0;
                }
                pCurPss->pTaskLock.unlock();
            }
            n = lws_service(context, nSleep);
        }
        lws_context_destroy(context);
    }

#if defined(ANDROID) || defined(__APPLE__)
    void startWSSV(int port, DebuggerAgent* pDbgAgent) {
        gpDbgAgent = pDbgAgent;
        interrupted = false;
        struct lws_context* context;
        // we're not using ssl
        const char* cert_path = NULL;
        const char* key_path = NULL;
        // no special options
        int opts = 0;


        lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_USER/* | LLL_INFO */ /* | LLL_DEBUG */, NULL);

        lws_context_creation_info cinfo;
        memset(&cinfo, 0, sizeof(cinfo));
        cinfo.port = port;
        //cinfo.mounts = &mount;
        cinfo.protocols = protocols;
        //cinfo.extensions = lws_get_internal_extensions();
        //if (!use_ssl) {
        cinfo.ssl_cert_filepath = NULL;
        cinfo.ssl_private_key_filepath = NULL;
        //} else {
        //  info.ssl_cert_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.pem";
        //  info.ssl_private_key_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.key.pem";
        //}
        cinfo.gid = -1;
        cinfo.uid = -1;
        // 设置TCP Keep-Alive参数
        //cinfo.ka_time = 60;  // 如果60秒内没有数据交换，就发送一个Keep-Alive探针
        //cinfo.ka_probes = 10; // 发送Keep-Alive探针的最大次数
        //cinfo.ka_interval = 10;  // 每个Keep-Alive探针之间间隔10秒        
        cinfo.timeout_secs = 3000;

        cinfo.options |= LWS_SERVER_OPTION_DISABLE_IPV6;


        // create libwebsocket context representing this server
        context = lws_create_context(&cinfo);

        if (context == NULL) {
            fprintf(stderr, "libwebsocket init failed\n");
            return;
        }

        printf("starting server...\n");
        wssvth = new std::thread(std::bind(wsserver_run, context));;
    }
#else
    void startWSSV(int port, DebuggerAgent* pDbgAgent) {
        gpDbgAgent = pDbgAgent;
        interrupted = false;
        struct lws_context *context;
        // we're not using ssl
        const char *cert_path = NULL;
        const char *key_path = NULL;
        // no special options
        int opts = 0;

        static const struct lws_http_mount mount = {
            /* .mount_next */               NULL,           /* linked-list "next" */
            /* .mountpoint */               "/",            /* mountpoint URL */
            /* .origin */                   ".",            /* serve from dir */
            /* .def */                      "index.html",   /* default filename */
            /* .protocol */                 NULL,
            /* .cgienv */                   NULL,
            /* .extra_mimetypes */          NULL,
            /* .interpret */                NULL,
            /* .cgi_timeout */              0,
            /* .cache_max_age */            0,
            /* .auth_mask */                0,
            /* .cache_reusable */           0,
            /* .cache_revalidate */         0,
            /* .cache_intermediaries */     0,
            /* .origin_protocol */          LWSMPRO_FILE,   /* files in a dir */
            /* .mountpoint_len */           1,              /* char count */
            /* .basic_auth_login_file */    NULL,
        };

        lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_USER/* | LLL_INFO */ /* | LLL_DEBUG */, NULL);

        lws_context_creation_info cinfo;
        memset(&cinfo, 0, sizeof(cinfo));
        cinfo.port = port;
        cinfo.mounts = &mount;
        cinfo.protocols = protocols;
        cinfo.extensions = lws_get_internal_extensions();
        //if (!use_ssl) {
        cinfo.ssl_cert_filepath = NULL;
        cinfo.ssl_private_key_filepath = NULL;
        //} else {
        //  info.ssl_cert_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.pem";
        //  info.ssl_private_key_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.key.pem";
        //}
        cinfo.gid = -1;
        cinfo.uid = -1;

        // create libwebsocket context representing this server
        context = lws_create_context( &cinfo);

        if (context == NULL) {
            fprintf(stderr, "libwebsocket init failed\n");
            return ;
        }

        printf("starting server...\n");
        wssvth  = new std::thread(std::bind(wsserver_run, context));;
    }
#endif
    void stopWSSV() {
        interrupted = true;
        if (wssvth != NULL)
        {
            wssvth->join();
        }
    }
}
#endif
