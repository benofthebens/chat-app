#ifndef SOCKET_H
#define SOCKET_H

#include <WinSock2.h>
#include <cstdint>
#include <memory>
#include <string>

struct Socket {
	std::string ip;
	uint16_t port = 0;
};

class NetworkSocket {
private:
	SOCKET handle_ = INVALID_SOCKET;
	Socket socket_{};
private:
    sockaddr_in MakeSocket() const;
public:
	explicit NetworkSocket();
	explicit NetworkSocket(const SOCKET handle) : handle_(handle) {}
	~NetworkSocket() { closesocket(handle_); }
	int Bind(const Socket& socket);
	int Listen(int backlog = 1) const;
	int Connect(const Socket& socket) ;
	std::unique_ptr<NetworkSocket> Accept() const;
	int Send(const void* data, int size);
	int Receive(void* buffer, int size);
	SOCKET GetHandle() const { return handle_; }
};

#endif