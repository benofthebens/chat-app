#include "client.h"
#include <iostream>
#include <thread>

int Client::start() {
    
    // Connect to the connection_socket_ with the service_
    if (connect(connection_socket_, reinterpret_cast<SOCKADDR*>(&service_), sizeof(service_)) == SOCKET_ERROR) {
        std::cout << "Failed to connect" << std::endl;
        return -1;
    }

    if (!message_sender_handler_) return -1;

    std::thread receiver(&Client::receiver, this);
    receiver.detach(); // detach

    std::thread sender(&Client::sender, this);
    sender.detach();

    return 0;
}
int Client::send_data(SOCKET server_socket, const char message[]) const {
    const int byte_count = send(server_socket, message, 256, 0);

    // Error checking 
    if (byte_count == SOCKET_ERROR) {
        std::cout << "Unable to send message" << std::endl;
        return -1;
    }

    return 0;
}
int Client::send_data(const char message[]) const {
    // Send the message to the receiver socket of length 200
    const int byte_count = send(connection_socket_, message, 256, 0);

    // Error checking 
    if (byte_count == SOCKET_ERROR) {
        std::cout << "Unable to send message" << std::endl;
        return -1;
    }

    return 0;
}
int Client::receiver() const {
    while (true) {
        char buffer[256]; // buffer to write to
        // wait for a message from the server
        const int byte_count = recv(connection_socket_, buffer, sizeof(buffer), 0);

        // Error checking
        if (byte_count < 0)
            break;

        // Output to client's console
        if (message_receiver_handler_)
            message_receiver_handler_(buffer);
    }
    return 0;
}
int Client::sender() {
    while (true) {
        if (ready_send_) {
            char buffer[256];
            const char* data = message_sender_handler_(buffer);
            ready_send_ = false;
            int _ = send_data(data);
        }
        Sleep(10);
    }
}
void Client::set_message_receiver_handler(std::function<void(const char[])> message_handler) {
    message_receiver_handler_ = message_handler;
}
void Client::set_message_sender_handler(std::function<const char*(char*)> message_handler) {
    message_sender_handler_ = message_handler;
}
void Client::send_request() {
    ready_send_ = true;
}

