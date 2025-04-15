#include "../../include/server.h"
#include <iostream>
#include "WinSock2.h"
#include <thread>

void Server::receive(SOCKET socket) {
    while (true) {
        char buffer[200];
        int byte_count = recv(socket, buffer, sizeof(buffer), 0);

        if (byte_count < 0) {
            std::cout << "Failed to get data" << std::endl;
            continue;
        }
        std::cout << buffer << std::endl;
        for (SOCKET client : clients) {
            if (client == socket) continue;
            this->send_msg(client, buffer);
        }
    }
}

int Server::start() {

    if (bind(connection_socket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cout << "Failed to bind socket" << std::endl;
        return -1;
    }
    if (listen(connection_socket, 1) == SOCKET_ERROR) {
        std::cout << "Failed to listen on the socket" << std::endl;
        return -1;
    }
    std::cout << "Server Running on the port on " << port << std::endl;

    SOCKET accept_socket;
    while (true) {
        accept_socket = accept(this->connection_socket, nullptr, nullptr);
        if (accept_socket == INVALID_SOCKET) {
            std::cout << "failed to accept" << std::endl;
            continue;
        }

        clients.push_back(accept_socket);

        std::thread receive(&Server::receive, this, accept_socket);
        receive.detach();
    }
}

int Server::send_msg(const SOCKET receiver_socket, const char message[]) {
    std::cout << sizeof(char) << std::endl;
    int byte_count = send(receiver_socket, message, sizeof(message), 0);
    if (byte_count == SOCKET_ERROR ) return -1;
    return 0;
}

int Server::broardcast_message(const char message[]) {
    for (SOCKET client : clients) {
        this->send_msg(client, message);
    }
    return 0;
}
