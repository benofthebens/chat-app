#include "network_core/connection.h"

int ServerConnection::Send(const void* data, const int size) {
    return socket_.Send(data, size);
}

int ServerConnection::Receive(void* buffer, const int size) {
    return socket_.Receive(buffer, size);
}

int ServerConnection::Bind(const Socket& socket) {
    return socket_.Bind(socket);
}

int ServerConnection::Listen(const int backlog) {
    const int result = socket_.Listen(backlog);
    listening_ = result != SOCKET_ERROR;
    return result;
}

std::unique_ptr<NetworkSocket> ServerConnection::Accept() const {
    return socket_.Accept();
}

int ClientConnection::Send(const void* data, const int size) {
    return socket_->Send(data, size);
}

int ClientConnection::Receive(void* buffer, const int size) {
    return socket_->Receive(buffer, size);
}

int ClientConnection::Connect(const Socket& socket) {
    const int result = socket_->Connect(socket);
    connected_ = result != SOCKET_ERROR;
    return result;
}

