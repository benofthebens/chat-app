#ifndef CONNECTION_BASE_H
#define CONNECTION_BASE_H

#include <WinSock2.h>
#include <string>
#include "message.h"

class ConnectionBase {
protected:
    sockaddr_in service_;
    SOCKET connection_socket_ = INVALID_SOCKET;
    WSAData wsa_data_;
    const std::string& host_;
    uint16_t port_;
public:
    ConnectionBase(const std::string& host, uint16_t port);
    virtual ~ConnectionBase() = default;
    virtual int Start() = 0;
    virtual int Shutdown() = 0;
};

#endif