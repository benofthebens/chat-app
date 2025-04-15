#pragma once
#include "connection_base.h"
#include <vector>
#include <WinSock2.h>

class Server : public ConnectionBase {
private:
    std::vector<SOCKET> clients;
    void Server::receive(SOCKET socket);
public:
    Server(
        const std::string& host,
        const unsigned int port
    ) : ConnectionBase(host, port) {
    }
    ~Server() = default;
    int start();
    int send_msg(const SOCKET reciever_socket, const char message[]) override;
    int broardcast_message(const char message[]);
};