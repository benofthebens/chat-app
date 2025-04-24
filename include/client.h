#ifndef CLIENT_H
#define CLIENT_H

#include "connection_base.h"
#include <functional>
#include <string>

class Client : public ConnectionBase {
private:
    std::function<void(const char[])> message_handler_;
public:
    Client(const std::string& host, const unsigned int port) : ConnectionBase(host, port) {}
    ~Client() override = default;
    int start();
    int send_data(SOCKET server_socket, const char message[]) const override;
    int send_data(const char message[]) const;
    int receiver() const;
    void set_message_handler(std::function<void(const char[])> message_handler);
};

#endif