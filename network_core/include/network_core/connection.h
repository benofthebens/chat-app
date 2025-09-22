#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>

#include "socket.h"

#define SOCKET_NOT_BOUND (-1)
#define SOCKET_NOT_CONNECTED (-1)

class IConnection {
public:
    virtual ~IConnection() = default;
    virtual int Send(const void* data, int size) = 0;
    virtual int Receive(void* buffer, int size) = 0;
    virtual SOCKET Handle() = 0;
};

class ServerConnection : public IConnection {
private:
    NetworkSocket socket_;
	bool bounded_ = false;
    bool listening_ = false;
public:
    ServerConnection() = default;
    ~ServerConnection() override = default;
    int Send(const void* data, int size) override;
    int Receive(void* buffer, int size) override;
    int Bind(const Socket& socket);
    int Listen(int backlog = 1);
    std::unique_ptr<NetworkSocket> Accept();
    bool IsListening() const { return listening_; }
    SOCKET Handle() override { return socket_.GetHandle(); }
};

class ClientConnection : public IConnection {
private:
    std::unique_ptr<NetworkSocket> socket_ = nullptr;
    bool connected_ = false;
public:
    ClientConnection()
        : socket_(std::make_unique<NetworkSocket>()) {}
    ClientConnection(std::unique_ptr<NetworkSocket> socket, const bool connected)
        : socket_(std::move(socket)), connected_(connected) {}
    ~ClientConnection() override = default;
    int Send(const void* data, int size) override;
    int Receive(void* buffer, int size) override;
    int Connect(const Socket& socket);
    bool IsConnected() const { return connected_; }
    SOCKET Handle() override { return socket_->GetHandle(); }
};

#endif