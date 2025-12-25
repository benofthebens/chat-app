#include "network_core/socket.h"

NetworkSocket::NetworkSocket() {
    handle_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

sockaddr_in NetworkSocket::MakeSocket() const {
    sockaddr_in service{};
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(socket_.ip.c_str());
    service.sin_port = htons(socket_.port);
    return service;
}

int NetworkSocket::Bind(const Socket& socket) {
    socket_ = socket;
    sockaddr_in service = MakeSocket();
    const int result = ::bind(handle_, reinterpret_cast<LPSOCKADDR>(&service), sizeof(service));

    return result;
}

int NetworkSocket::Listen(const int backlog) const {
    return ::listen(handle_, backlog);
}

int NetworkSocket::Connect(const Socket& socket) {
    socket_ = socket;
    sockaddr_in service = MakeSocket();

    const int result = ::connect(handle_, reinterpret_cast<LPSOCKADDR>(&service), sizeof(service));
    return result;
}

std::unique_ptr<NetworkSocket> NetworkSocket::Accept() const {
    const SOCKET client_handle = accept(handle_, nullptr, nullptr);

    if (client_handle == INVALID_SOCKET) {
        return nullptr;
    }

    return std::make_unique<NetworkSocket>(client_handle);
}

int NetworkSocket::Send(const void* data, const int size) const {
    return send(handle_, static_cast<const char*>(data), size, 0);
}

int NetworkSocket::Receive(void* buffer, const int size) const {
    return recv(handle_, static_cast<char*>(buffer), size, 0);
}

void NetworkSocket::Close() {
    closesocket(handle_);
    socket_ = {};
}



