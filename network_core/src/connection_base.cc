#include "connection_base.h"
#include <WinSock2.h>
#include <string>
#include <stdexcept>

ConnectionBase::ConnectionBase(const std::string& host, const uint16_t port) : host_(host), port_(port) {
    // Setup WSA
    constexpr WORD version_requested = MAKEWORD(2, 2); // gets the version requested

    if (WSAStartup(version_requested, &this->wsa_data_) != 0) {
        throw std::runtime_error("Unable to find dll");
    }

    // Setup Socket
    connection_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (connection_socket_ == INVALID_SOCKET) {
        throw std::runtime_error("Unable to setup socket");
    }
    
    // Set up the service_
    service_.sin_family = AF_INET;
    service_.sin_addr.s_addr = inet_addr(host.c_str());
    service_.sin_port = htons(port);
}
