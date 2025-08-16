#include "server.h"
#include <iostream>
#include "WinSock2.h"
#include <thread>
#include <algorithm>

int Server::Start() {

    // Bind the connection socket to the service_
    if (bind(connection_socket_, reinterpret_cast<SOCKADDR*>(&service_), sizeof(service_)) == SOCKET_ERROR) {
        std::cout << "Failed to bind socket" << "\n";
        return -1;
    }
    // Listen on the bound socket
    if (listen(connection_socket_, 1) == SOCKET_ERROR) {
        std::cout << "Failed to listen on the socket" << "\n";
        return -1;
    }

    std::thread listen(&Server::Connect, this);
    listen.detach();

    return 0;
}

int Server::Shutdown() {
    running_ = false;
    closesocket(connection_socket_);

    for (const SOCKET client : clients_) {
        closesocket(client);
    }

    clients_.clear();
    return 0;
}

void Server::Receive(const SOCKET socket) {
    while (running_) {
        // buffer to be read into
        Message msg;
        // Waits for the client to send a request
        const int byte_count = recv(socket, reinterpret_cast<char*>(&msg), kMaxDataSize, 0);

        if (byte_count < 0) break;

        msg.sender = Sender::kUser;
        // Broadcast message to everyone 
        int _ = Broadcast(&msg, socket);
    }
    // Remove the socket from the clients
    clients_.erase(std::remove(clients_.begin(), clients_.end(), socket),clients_.end());

    Message msg;
    strcpy_s(msg.data, "A User has disconnected from the session");
    msg.sender = Sender::kServer;

    int _ = Broadcast(&msg);
    closesocket(socket);
}

void Server::Connect() {
    std::cout << "Server Running on the port on " << port_ << "\n";

    while (running_) {
        // Waits for accept of connections
        SOCKET accept_socket = accept(connection_socket_, nullptr, nullptr);

        if (accept_socket == INVALID_SOCKET) {
            std::cout << "failed to accept" << "\n";
            continue;
        }

        std::cout << "Client has connected to server" << "\n";

        // put it in the list of clients connected
        clients_.push_back(accept_socket);

        Message msg;
        strcpy_s(msg.data, "A User has connected to the session");
        msg.sender = Sender::kServer;

        int _ = Broadcast(&msg);

        // Create thread per user connected 
        std::thread receive(&Server::Receive, this, accept_socket);
        receive.detach(); // detaches the thread
    }
}

int Server::SendData(const SOCKET client_socket, Message* message) const {
    // Send a message to the receiver socket with a length of 200
    const int byte_count = send(client_socket, reinterpret_cast<const char*>(message), 256, 0);

    if (byte_count == SOCKET_ERROR) {
        return SOCKET_ERROR;
    }

    return 0;
}

int Server::Broadcast(Message* message, SOCKET sender_socket) const {
    // Send message to every client
    for (const SOCKET client : clients_) {
        message->is_sender = (sender_socket == client);
        if (SendData(client, message) == SOCKET_ERROR) { return -1; }
    }

    return 0;
}
