#ifndef SERVER_H
#define SERVER_H

#include <atomic>

#include "connection_base.h"
#include "message.h"
#include <vector>
#include <WinSock2.h>

class Server : public ConnectionBase {
private:
    std::vector<SOCKET> clients_;
    std::atomic<bool> running_{ true };
    void Receive(SOCKET socket);
    void Connect();
public:
    Server(
        const std::string& host,
        const unsigned int port
    ) : ConnectionBase(host, port) {}
    ~Server() override = default;
    int Start() override;
    int Shutdown() override;
    int SendData(SOCKET client_socket, Message* message) const;
    int Broadcast(Message* message, SOCKET sender_socket = INVALID_SOCKET) const;
    bool is_running() { return running_; }
};

#endif