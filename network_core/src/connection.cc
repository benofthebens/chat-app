#include "network_core/connection.h"

int ServerConnection::Send(const void* data, const int size) {
    if (!bounded_) {
        return SOCKET_NOT_BOUND;
    }
    return socket_.Send(data, size);
}

int ServerConnection::Receive(void* buffer, const int size) {
    if (!bounded_) {
        return SOCKET_NOT_BOUND;
    }
    return socket_.Receive(buffer, size);
}

int ServerConnection::Bind(const Socket& socket) {
    if (bounded_) {
        return -1;
    }
    int result = socket_.Bind(socket);
    bounded_ = true;
    return 0;
}

int ServerConnection::Listen(const int backlog) {
    if (!bounded_) {
        return SOCKET_NOT_BOUND;
    }
    const int result = socket_.Listen(backlog);
    listening_ = true;
    return result;
}
std::unique_ptr<NetworkSocket> ServerConnection::Accept() {
    if (!listening_) {
        return std::make_unique<NetworkSocket>(INVALID_SOCKET);
    }
    return socket_.Accept();
}

int ClientConnection::Send(const void* data, const int size) {
    if (!connected_) {
        return SOCKET_NOT_CONNECTED;
    }
    return socket_->Send(data, size);
}

int ClientConnection::Receive(void* buffer, const int size) {
    if (!connected_) {
        return SOCKET_NOT_CONNECTED;
    }
    return socket_->Receive(buffer, size);
}

int ClientConnection::Connect(const Socket& socket) {
    if (connected_) {
        return -1;
    }
    socket_->Connect(socket);
    connected_ = true;
    return 0;
}

