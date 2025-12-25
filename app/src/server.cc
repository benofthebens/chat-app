
#include <iostream>

#include "app/network_events.h"
#include "network_core/application_server.h"

int main() {
    constexpr WORD version_requested = MAKEWORD(2, 2); // gets the version requested
    WSADATA wsa_data;
    if (WSAStartup(version_requested, &wsa_data) != 0) { return -1; }

    ApplicationServer<Message> server;
    Socket socket = { "127.0.0.1", 8080 };
    server.on_connect_ = [&server](Session<Message>& session) { server.SendAll({ "User Joined Session" }); };
    server.on_message_ = [&server](Session<Message>& session, const Message& msg) {
        server.SendAll(msg);
    };
    server.Run(socket);
}
