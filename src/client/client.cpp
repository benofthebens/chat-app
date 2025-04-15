#include "../../include/client.h"
#include <iostream>
#include <thread>

int Client::start() {
   if (connect(connection_socket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cout << "Failed to conenct" << std::endl;
        return -1;
   }
   while (true) {
        char buffer[200];

        std::thread receiver(&Client::receiver, this, connection_socket);
        receiver.detach();

        std::cin.getline(buffer, 200);
        send_msg(connection_socket, buffer);
   }
    return 0;
}
int Client::send_msg(const SOCKET reciever_socket, const char message[])  {

    int byte_count = send(reciever_socket, message, sizeof(message), 0);

    if (byte_count == SOCKET_ERROR) {
        std::cout << "Unable to send message" << std::endl;
        return -1;
    }

}

int Client::receiver(SOCKET socket) {
    while (true) {
        char buffer[200];
        int byte_count = recv(connection_socket, buffer, sizeof(buffer), 0);
        if (byte_count < 0) {
            std::cout << "Failed to get data" << std::endl;
            continue;
        }
        std::cout << buffer << std::endl;
    }
    return 0;
}
