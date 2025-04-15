#pragma comment(lib, "Ws2_32.lib")

#include "client.h"

int main() {
    Client client("127.0.0.1", 8080);
    client.start();
    return 0;
}