#pragma comment(lib, "Ws2_32.lib")

#include "../../include/server.h"
#include <WinSock2.h>
#include <iostream>

int main() {
    Server server("127.0.0.1", 8080);
    server.start();
    return 0;
}