#include "client.h"
#include <iostream>
#include <thread>

int Client::start() {
    
    // Connect to the connection_socket_ with the service_
    if (connect(connection_socket_, reinterpret_cast<SOCKADDR*>(&service_), sizeof(service_)) == SOCKET_ERROR) {
        std::cout << "Failed to connect" << std::endl;
        return -1;
    }

    while (true) {
        // Create buffer with 200 size to get message
        char buffer[200];

        // Create receiver thread to get responses from server
        std::thread receiver(&Client::receiver, this);
        receiver.detach(); // detach

        // Read message from cli
        std::cin.getline(buffer, 200);
        send_msg(connection_socket_, buffer);
    }
}

int Client::send_msg(const SOCKET receiver_socket, const char message[]) {
    // Send the message to the receiver socket of length 200
    const int byte_count = send(receiver_socket, message, 200, 0);

    // Error checking 
    if (byte_count == SOCKET_ERROR) {
        std::cout << "Unable to send message" << std::endl;
        return -1;
    }

    return 0;
}

int Client::receiver() const {
    while (true) {
        char buffer[200]; // buffer to write to
        // wait for a message from the server
        const int byte_count = recv(connection_socket_, buffer, sizeof(buffer), 0);

        // Error checking
        if (byte_count < 0) {
            std::cout << "Failed to get data" << std::endl;
            continue;
        }

        // Output to client's console
        std::cout << buffer << std::endl;
    }
}
