#include "../../include/client.h"
#include <iostream>
#include <thread>
#include <array>

int Client::start() {
    
    // Connect to the connection_socket with the service
    if (connect(connection_socket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cout << "Failed to conenct" << std::endl;
        return -1;
    }

    while (true) {
        // Create buffer with 200 size to get message
        char buffer[200];

        // Create receiver thread to get response from server
        std::thread receiver(&Client::receiver, this, connection_socket);
        receiver.detach(); // detach

        // Read message from cli
        std::cin.getline(buffer, 200);
        send_msg(connection_socket, buffer);
    }

    return 0;
}

int Client::send_msg(const SOCKET reciever_socket, const char message[]) {
    // Send the message to the receiver socket of length 200
    int byte_count = send(reciever_socket, message, 200, 0);

    // Error checking 
    if (byte_count == SOCKET_ERROR) {
        std::cout << "Unable to send message" << std::endl;
        return -1;
    }

    return 0;
}

int Client::receiver(SOCKET socket) {
    while (true) {
        char buffer[200]; // buffer to write to
        // wait for a message from the server
        int byte_count = recv(connection_socket, buffer, sizeof(buffer), 0);

        // Error checking
        if (byte_count < 0) {
            std::cout << "Failed to get data" << std::endl;
            continue;
        }

        // Ouput to client's console
        std::cout << buffer << std::endl;
    }
    return 0;
}
