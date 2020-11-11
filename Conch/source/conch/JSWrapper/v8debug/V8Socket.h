#ifndef V8_SOCKET_H_
#define V8_SOCKET_H_
#ifdef WIN32
#include <WinSock2.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
typedef unsigned long     SOCKET;
#endif

typedef unsigned long TcpSocketHandle;
#define __INVALID_SOCKET	 (TcpSocketHandle)(~0)

#define ASSERT(condition)      ((void) 0)
typedef int int32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;  // NOLINT
//typedef long long int64_t;  

namespace laya {
	class V8ChromeDebugMutex;

	class V8Socket {
	public:
		virtual ~V8Socket() {}

		// Server initialization.
		virtual bool Bind(const int port) = 0;
		virtual bool Listen(int backlog) const = 0;
		virtual V8Socket* Accept() const = 0;

		// Client initialization.
		virtual bool Connect(const char* host, const char* port) = 0;

		// Shutdown socket for both read and write. This causes blocking Send and
		// Receive calls to exit. After Shutdown the Socket object cannot be used for
		// any communication.
		virtual bool Shutdown() = 0;

		// Data Transimission
		// Return 0 on failure.
		virtual int Send(const char* data, int len) const = 0;
		virtual int Receive(char* data, int len) const = 0;

		// Set the value of the SO_REUSEADDR socket option.
		virtual bool SetReuseAddress(bool reuse_address) = 0;

		virtual bool IsValid() const = 0;

		static bool SetUp();
		static int LastError();
		static uint16_t HToN(uint16_t value);
		static uint16_t NToH(uint16_t value);
		static uint32_t HToN(uint32_t value);
		static uint32_t NToH(uint32_t value);
	};
	class Win32Socket : public V8Socket {
	public:
		explicit Win32Socket() {
			// Create the socket.
			socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		}
		explicit Win32Socket(SOCKET socket) : socket_(socket) { }
		virtual ~Win32Socket() { Shutdown(); }

		// Server initialization.
		bool Bind(const int port);
		bool Listen(int backlog) const;
		V8Socket* Accept() const;

		// Client initialization.
		bool Connect(const char* host, const char* port);

		// Shutdown socket for both read and write.
		bool Shutdown();

		// Data Transimission
		int Send(const char* data, int len) const;
		int Receive(char* data, int len) const;

		bool SetReuseAddress(bool reuse_address);

		bool IsValid() const { return socket_ != __INVALID_SOCKET; }

	private:
		SOCKET socket_;
	};
}
#endif  // V8_PLATFORM_H_

