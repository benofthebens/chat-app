#pragma comment(lib, "Ws2_32.lib")

#include <iostream>

#include "client.h"

int main() {
    Client client("127.0.0.1", 8080);
    client.set_message_handler([](const char x[]) { std::cout << x << std::endl;});
    client.start();
    return 0;
}