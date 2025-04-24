#ifndef CLIENT_H
#define CLIENT_H

#include "connection_base.h"
#include <functional>
#include <string>

class Client : public ConnectionBase {
private:
    std::function<void(const char[])> message_receiver_handler_;
    std::function<const char*(char*)> message_sender_handler_;
    bool ready_send_ = false;
    int receiver() const;
    int sender();
public:
    Client(const std::string& host, const unsigned int port) : ConnectionBase(host, port) {}
    ~Client() override = default;
    int start();
    int send_data(SOCKET server_socket, const char message[]) const override;
    int send_data(const char message[]) const;
    void send_request();
    void set_message_receiver_handler(std::function<void(const char[])> message_handler);
    void set_message_sender_handler(std::function<const char*(char*)> message_handler);
};

#endif