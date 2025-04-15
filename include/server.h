#pragma once
#include "connection_base.h"
#include <vector>
#include <WinSock2.h>

class Server : public ConnectionBase {
private:
    std::vector<SOCKET> clients_;
    void receive(const SOCKET socket);
public:
    Server(
        const std::string& host,
        const unsigned int port
    ) : ConnectionBase(host, port) {}
    ~Server() override = default;
    int start();
    int send_msg(const SOCKET receiver_socket, const char message[]) override;
    int broadcast_message(const char message[]);
};