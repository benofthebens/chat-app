#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <iostream>
#include "../../include/client.h"

int main() {
    Client client("127.0.0.1", 8080);
    client.start();

 
    // Send a message to the client
    return 0;
}