#ifndef CONNECTION_BASE_H
#define CONNECTION_BASE_H

#include <WinSock2.h>
#include <string>

class ConnectionBase {
protected:
    sockaddr_in service_;
    SOCKET connection_socket_ = INVALID_SOCKET;
    WSAData wsa_data_;
    const std::string& host_;
    const unsigned int port_;
public:
    ConnectionBase(const std::string& host, const unsigned int port);
    virtual ~ConnectionBase() = default;
    virtual int send_msg(const SOCKET receiver_socket, const char message[]) = 0;
};

#endif