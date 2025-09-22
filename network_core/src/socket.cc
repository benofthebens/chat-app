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

    if (::bind(handle_, reinterpret_cast<LPSOCKADDR>(&service), sizeof(service)) == SOCKET_ERROR) {
        return SOCKET_ERROR;
    }

    return 0;
}

int NetworkSocket::Listen(const int backlog) const {
    if (::listen(handle_, backlog) == SOCKET_ERROR) {
        return SOCKET_ERROR;
    }
    return 0;
}

int NetworkSocket::Connect(const Socket& socket) {
    socket_ = socket;
    sockaddr_in service = MakeSocket();

    if (::connect(handle_, reinterpret_cast<LPSOCKADDR>(&service), sizeof(service)) == SOCKET_ERROR) {
        return SOCKET_ERROR;
    }
    return 0;
}

std::unique_ptr<NetworkSocket> NetworkSocket::Accept() const {
    const SOCKET client_handle = accept(handle_, nullptr, nullptr);
    if (client_handle == SOCKET_ERROR) {
        return std::make_unique<NetworkSocket>(INVALID_SOCKET);
    }
    return std::make_unique<NetworkSocket>(client_handle);
}

int NetworkSocket::Send(const void* data, const int size) {
    const int byte_count = send(handle_, static_cast<const char*>(data), size, 0);
    if (byte_count == SOCKET_ERROR) {
        return SOCKET_ERROR;
    }
    return byte_count;
}

int NetworkSocket::Receive(void* buffer, const int size) {
    const int byte_count = recv(handle_, static_cast<char*>(buffer), size, 0);
    return byte_count;
}



