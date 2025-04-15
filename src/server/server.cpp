#include "../../include/server.h"
#include <iostream>
#include "WinSock2.h"
#include <thread>

void Server::receive(SOCKET socket) {
    while (true) {
        // buffer to be read into
        char buffer[200];
        // Waits for the client to send a request
        int byte_count = recv(socket, buffer, sizeof(buffer), 0);

        if (byte_count < 0) {
            std::cout << "Failed to get data" << std::endl;
            continue;
        }

        std::cout << byte_count << std::endl;

        // Broadcast message to everyone except the client sent
        for (SOCKET client : clients) {
            if (client == socket) continue;
            this->send_msg(client, buffer);
        }
    }
}

int Server::start() {

    // Bind the connection socket to the service
    if (bind(connection_socket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cout << "Failed to bind socket" << std::endl;
        return -1;
    }

    // Listen on the the binded socket
    if (listen(connection_socket, 1) == SOCKET_ERROR) {
        std::cout << "Failed to listen on the socket" << std::endl;
        return -1;
    }

    std::cout << "Server Running on the port on " << port << std::endl;

    SOCKET accept_socket;
    while (true) {

        // Waits for accept of connections
        accept_socket = accept(this->connection_socket, nullptr, nullptr);
        if (accept_socket == INVALID_SOCKET) {
            std::cout << "failed to accept" << std::endl;
            continue;
        }

        // put it in the list of clients connected
        clients.push_back(accept_socket);

        // Create thread per user connected 
        std::thread receive(&Server::receive, this, accept_socket);
        receive.detach(); // detaches the thread
    }
}

int Server::send_msg(const SOCKET receiver_socket, const char message[]) {
    // Send a message to the receiver socket with a length of 200
    int byte_count = send(receiver_socket, message, 200, 0);
    if (byte_count == SOCKET_ERROR ) return -1;
    return 0;
}

int Server::broardcast_message(const char message[]) {
    // Send message to every client
    for (SOCKET client : clients) {
        this->send_msg(client, message);
    }
    return 0;
}
