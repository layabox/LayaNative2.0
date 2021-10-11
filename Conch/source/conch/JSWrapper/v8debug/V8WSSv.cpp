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
    参照 https://github.com/warmcat/libwebsockets/tree/master/minimal-examples/ws-server/minimal-ws-server-echo
    需要切换版本。
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
        case LWS_CALLBACK_ESTABLISHED:  //有人连进来了
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

        case LWS_CALLBACK_SERVER_WRITEABLE: //可以发送了
            //缺省的是继续发送。例如上一次可能只发送了一部分
            n = LWS_WRITE_CONTINUATION;
            if (!pss->continuation) {
                //如果是新的任务，就设置文本或者二进制。
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
            //一次处理一个
            if (pss->pSendTask.size() > 0) {
                std::string& t1 = pss->pSendTask.front();
                //printf("send:%s\n", t1.substr(0,200).c_str());
                sendMsgLen = t1.length();
                if (pss->pSendBuff) {
                    delete[] pss->pSendBuff;
                }
                pss->pSendBuff = new unsigned char[sendMsgLen + LWS_PRE];
                memcpy(pss->pSendBuff + LWS_PRE, t1.c_str(), sendMsgLen);
                //删掉第一个
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
                    //这里不知道怎么处理，按理说部分发送的情况不用管。下次LWS_CALLBACK_SERVER_WRITEABLE的时候应该已经全部发送了。
                    //所以pss->continuation应该还是=0
                    //这里直接返回，这样pss->continuation就是1了，是故意的么
                    return -1;
                }
            }
            if (pss->final)
                pss->continuation = 0;
            /* 这会导致websocket报错，写返回-1
            //一次全部处理
            pss->pTaskLock.lock();
            if (pss->pSendTask.size() > 0) {
                for (std::string& t1 : pss->pSendTask) {
                    printf("send:%s\n", t1.c_str());
                    sendMsgLen = t1.length();
                    if (pss->pSendBuff) {
                        delete[] pss->pSendBuff;//TODO 优化
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
                        //这里不知道怎么处理，按理说部分发送的情况不用管。下次LWS_CALLBACK_SERVER_WRITEABLE的时候应该已经全部发送了。
                        //所以pss->continuation应该还是=0
                        //这里直接返回，这样pss->continuation就是1了，是故意的么
                        return -1;
                    }
                }
            }
            pss->pTaskLock.unlock();
            */
            //lws_rx_flow_control(wsi, 1);    //允许在此连接上接收数据。
            lws_callback_on_writable(wsi);//一旦有机会，触发写回调
            break;
        case LWS_CALLBACK_RECEIVE: { // 可以接收了。
                                     // Create a buffer to hold our response
                                     // it has to have some pre and post padding.
                                     // You don't need to care what comes there, libwebsockets
                                     // will do everything for you. For more info see
                                     // http://git.warmcat.com/cgi-bin/cgit/libwebsockets/tree/lib/libwebsockets.h#n597

            //amsg.first = lws_is_first_fragment(wsi);
            pss->final = lws_is_final_fragment(wsi);    //是否是最后一个信息。
            pss->binary = lws_frame_is_binary(wsi);     //是否是二进制的。
            const size_t remaining = lws_remaining_packet_payload(wsi);
            //lwsl_info("+++ test-echo: RX len %ld final %ld, pss->len=%ld\n",(long)len, (long)pss->final, (long)pss->len);

            if (in && len > 0) {
                if (remaining > 0) {
                    //帧不完整。 TODO
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

            //lws_rx_flow_control(wsi, 0);//禁止在此连接上接收数据。
            lws_callback_on_writable(wsi);//一旦有机会，触发写回调
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
            //nWSSVSleep 是 timeout_ms: 等待超时时间，即没有找到需要处理的连接需要等待的时间，为0则立即返回；
            //这个会一通过回调处理消息。
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

    void stopWSSV() {
        interrupted = true;
        if (wssvth != NULL)
        {
            wssvth->join();
        }
    }
}
#endif
