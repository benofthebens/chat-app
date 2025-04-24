#include "client.h"
#include <iostream>
#include <thread>

int Client::start() {
    
    // Connect to the connection_socket_ with the service_
    if (connect(connection_socket_, reinterpret_cast<SOCKADDR*>(&service_), sizeof(service_)) == SOCKET_ERROR) {
        std::cout << "Failed to connect" << std::endl;
        return -1;
    }

    std::thread receiver(&Client::receiver, this);
    receiver.detach(); // detach

    while (true) {
        // Create buffer with 200 size to get message
        char buffer[256];

        // Read message from cli
        std::cin.getline(buffer, sizeof(buffer));
        //send_msg(connection_socket_, buffer);
    }
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
        if (byte_count < 0) {
            std::cout << "Failed to get data" << std::endl;
            continue;
        }

        // Output to client's console
        if (message_handler_)
            message_handler_(buffer);
    }
}
void Client::set_message_handler(std::function<void(const char[])> message_handler) {
    message_handler_ = message_handler;
}
