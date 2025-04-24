#include "server.h"
#include <iostream>
#include "WinSock2.h"
#include <thread>
#include <algorithm>

int Server::start() {

    // Bind the connection socket to the service_
    if (bind(connection_socket_, reinterpret_cast<SOCKADDR*>(&service_), sizeof(service_)) == SOCKET_ERROR) {
        std::cout << "Failed to bind socket" << std::endl;
        return -1;
    }

    // Listen on the bound socket
    if (listen(connection_socket_, 1) == SOCKET_ERROR) {
        std::cout << "Failed to listen on the socket" << std::endl;
        return -1;
    }

    std::cout << "Server Running on the port on " << port_ << std::endl;

    while (true) {
        // Waits for accept of connections
        SOCKET accept_socket = accept(connection_socket_, nullptr, nullptr);
        if (accept_socket == INVALID_SOCKET) {
            std::cout << "failed to accept" << std::endl;
            continue;
        }
        std::cout << "Client has connected to server" << std::endl;

        // put it in the list of clients connected
        clients_.push_back(accept_socket);
        int _ = broadcast_message("User has joined the Chat");

        // Create thread per user connected 
        std::thread receive(&Server::receive, this, accept_socket);
        receive.detach(); // detaches the thread
    }
}

void Server::receive(const SOCKET socket) {
    while (true) {
        // buffer to be read into
        char buffer[256];
        // Waits for the client to send a request
        const int byte_count = recv(socket, buffer, sizeof(buffer), 0);

        if (byte_count < 0) break;

        // Broadcast message to everyone except the client sent
        int _ = broadcast_message(buffer);
   }
    // Remove the socket from the clients
    clients_.erase(
        std::remove(
            clients_.begin(), 
            clients_.end(), 
            socket
        ), 
        clients_.end()
    );

    int _ = broadcast_message("A User has disconnected from the session");
}

int Server::send_data(const SOCKET client_socket, const char message[]) const {
    // Send a message to the receiver socket with a length of 200
    const int byte_count = send(client_socket, message, 256, 0);
    if (byte_count == SOCKET_ERROR) 
        return SOCKET_ERROR;

    return 0;
}

int Server::broadcast_message(const char message[]) const {
    // Send message to every client
    for (const SOCKET client : clients_) {
        if (send_data(client, message) == SOCKET_ERROR) return -1;
    }

    return 0;
}
