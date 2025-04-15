#pragma once
#include <WinSock2.h>
#include <string>

class ConnectionBase {
protected:
    sockaddr_in service;
    SOCKET connection_socket = INVALID_SOCKET;
    WSAData wsa_data;
    const std::string& host;
    const unsigned int port;
public:
    ConnectionBase(const std::string& host, const unsigned int port);
    virtual ~ConnectionBase() = default;
    virtual int send_msg(const SOCKET reciever_socket, const char message[]) = 0;
};