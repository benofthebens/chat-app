#include "../include/connection_base.h"
#include <WinSock2.h>
#include <string>
#include <stdexcept>

ConnectionBase::ConnectionBase(const std::string& host, const unsigned int port) : host(host), port(port) {
    // Setup WSA
    WORD w_version_requested = MAKEWORD(2, 2); // gets the version requested
    if (WSAStartup(w_version_requested, &this->wsa_data) != 0)
        throw std::runtime_error("Unable to find dll");

    // Setup Socket
    this->connection_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->connection_socket == INVALID_SOCKET) 
        throw std::runtime_error("Unable to setup socket");

    this->service.sin_family = AF_INET;
    this->service.sin_addr.s_addr = inet_addr(host.c_str());
    this->service.sin_port = htons(port);
}
