// ----------------------------------------------------------------------------
// Win32 socket support.
//
#include "V8Socket.h"
#include <memory>

#ifdef _WIN32
#else
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

//using namespace v8;
namespace laya {
	bool Win32Socket::Bind(const int port) {
		if (!IsValid()) {
			return false;
		}

		sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(port);
		int status = bind(socket_,
			reinterpret_cast<struct sockaddr *>(&addr),
			sizeof(addr));
		return status == 0;
	}

	bool Win32Socket::Listen(int backlog) const {
		if (!IsValid()) {
			return false;
		}

		int status = listen(socket_, backlog);
		return status == 0;
	}

	V8Socket* Win32Socket::Accept() const {
		if (!IsValid()) {
			return NULL;
		}

		SOCKET socket = accept(socket_, NULL, NULL);
		if (socket == __INVALID_SOCKET) {
			return NULL;
		}
		else {
			return new Win32Socket(socket);
		}
	}

	bool Win32Socket::Connect(const char* host, const char* port) {
		return false;
		/*
		if (!IsValid()) {
			return false;
		}

		// Lookup host and port.
		struct addrinfo *result = NULL;
		struct addrinfo hints;
		memset(&hints, 0, sizeof(addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		int status = getaddrinfo(host, port, &hints, &result);
		if (status != 0) {
			return false;
		}

		// Connect.
		status = connect(socket_,
			result->ai_addr,
			static_cast<int>(result->ai_addrlen));
		freeaddrinfo(result);
		return status == 0;
		*/
	}

	bool Win32Socket::Shutdown() {
		if (IsValid()) {
			// Shutdown socket for both read and write.
			int status = shutdown(socket_, 2);// SD_BOTH);
#ifdef _WIN32
			closesocket(socket_);
#else
			close(socket_);
#endif
			socket_ = __INVALID_SOCKET;
			return status == -1;
		}
		return true;
	}

	int Win32Socket::Send(const char* data, int len) const {
		if (len <= 0) return 0;
		int written = 0;
		while (written < len) {
			int status = send(socket_, data + written, len - written, 0);
			if (status == 0) {
				break;
			}
			else if (status > 0) {
				written += status;
			}
			else {
				return 0;
			}
		}
		return written;
	}

	int Win32Socket::Receive(char* data, int len) const {
		if (len <= 0) return 0;
		int status = recv(socket_, data, len, 0);
		return (status == -1) ? 0 : status;
	}


	bool Win32Socket::SetReuseAddress(bool reuse_address) {
		bool on = reuse_address ? true : false;
		int status = setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR,
			reinterpret_cast<char*>(&on), sizeof(on));
		return status == -1;
	}

	bool V8Socket::SetUp() {
		// Initialize Winsock32
#ifdef _WIN32
		int err;
		WSADATA winsock_data;
		WORD version_requested = MAKEWORD(1, 0);
		err = WSAStartup(version_requested, &winsock_data);
		if (err != 0) {
			//v8::internal::PrintF("Unable to initialize Winsock, err = %d\n", Socket::LastError());
		}

		return err == 0;
#endif
		return true;
	}

	int V8Socket::LastError() {
#ifdef _WIN32
		return WSAGetLastError();
#endif
		return 0;
	}

	uint16_t V8Socket::HToN(uint16_t value) {
		return htons(value);
	}

	uint16_t V8Socket::NToH(uint16_t value) {
		return ntohs(value);
	}

	uint32_t V8Socket::HToN(uint32_t value) {
		return htonl(value);
	}

	uint32_t V8Socket::NToH(uint32_t value) {
		return ntohl(value);
	}
}
