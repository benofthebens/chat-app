#pragma once

#include "connection_base.h"

class Client : public ConnectionBase {
private:
public:
    Client(const std::string& host, const unsigned int port) : ConnectionBase(host, port) {}
    ~Client() = default;
    int start();
    int send_msg(const SOCKET reciever_socket, const char message[]) override;
    int receiver(SOCKET socket);
};