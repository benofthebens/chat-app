#ifndef CLIENT_H
#define CLIENT_H

#include <atomic>

#include "connection_base.h"
#include <functional>
#include <string>

class Client : public ConnectionBase {
private:
    std::function<void(Message*)> message_receiver_;
    std::function<void(Message*)> message_sender_;
    std::atomic<bool> running_{ false };
    std::atomic<bool> sending_{false};
private:
    int Receiver() const;
    int Sender();
public:
    Client(const std::string& host, const unsigned int port) : ConnectionBase(host, port) {}
    ~Client() override = default;
    int Start() override;
    int Shutdown() override;
    int SendData(SOCKET server_socket, const char message[]) const;
    int SendData(Message* message) const;
    void SendRequest() { sending_ = true; }
    bool is_running() { return running_; }
    void set_message_receiver(std::function<void(Message*)> message_handler) {
        message_receiver_ = std::move(message_handler);
    }
    void set_message_sender(std::function<void(Message*)> message_handler) {
        message_sender_ = std::move(message_handler);
    }
};

#endif