#pragma once

#include "connection_base.h"

class Client : public ConnectionBase {
private:
public:
    Client(const std::string& host, const unsigned int port) : ConnectionBase(host, port) {}
    ~Client() override = default;
    int start();
    int send_msg(const SOCKET receiver_socket, const char message[]) override;
    int receiver() const;
};