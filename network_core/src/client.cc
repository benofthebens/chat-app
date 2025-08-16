#include "client.h"
#include <iostream>
#include <thread>

int Client::Start() {
    
    // Connect to the connection_socket_ with the service_
    if (connect(connection_socket_, reinterpret_cast<SOCKADDR*>(&service_), sizeof(service_)) == SOCKET_ERROR) {
        std::cout << "Failed to connect" << "\n";
        return -1;
    }

    if (!message_sender_handler_) { return -1; }

    running_ = true;

    std::thread receiver(&Client::Receiver, this);
    receiver.detach(); // detach

    std::thread sender(&Client::Sender, this);
    sender.detach();

    return 0;
}

int Client::Shutdown() {
    running_ = false;
    closesocket(connection_socket_);
    return 0;
}

int Client::SendData(SOCKET server_socket, const char message[]) const {
    const int byte_count = send(server_socket, message, 256, 0);

    // Error checking 
    if (byte_count == SOCKET_ERROR) {
        std::cout << "Unable to send message" << "\n";
        return -1;
    }

    return 0;
}
int Client::SendData(Message* message) const {
    // Send the message to the receiver socket of length 200
    const int byte_count = send(connection_socket_, reinterpret_cast<char*>(message), sizeof(Message), 0);

    // Error checking 
    if (byte_count == SOCKET_ERROR) {
        std::cout << "Unable to send message" << "\n";
        return -1;
    }

    return 0;
}
int Client::Receiver() const {
    while (running_) {
        Message msg;
        // wait for a message from the server
        const int byte_count = recv(connection_socket_, reinterpret_cast<char*>(&msg), sizeof(Message), 0);

        // Error checking
        if (byte_count < 0) { break; }

        // Output to client's console
        if (message_receiver_handler_) {
            message_receiver_handler_(&msg);
        }
    }
    return 0;
}
int Client::Sender() {
    while (running_) {
        if (sending_) {
            Message msg;
            message_sender_handler_(&msg);
            sending_ = false;
            int _ = SendData(&msg);
        }
        Sleep(10);
    }
    return 0;
}
