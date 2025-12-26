
#include <iostream>

#include "app/network_events.h"
#include "network/application_server.h"

int main() {
    constexpr WORD version_requested = MAKEWORD(2, 2); // gets the version requested
    WSADATA wsa_data;
    if (WSAStartup(version_requested, &wsa_data) != 0) { return -1; }
    auto protocol = std::make_shared<PodProtocol<Message>>();
    ApplicationServer server(protocol);

    Socket socket = { "127.0.0.1", 8080 };
    server.on_connect_ = [&server, &protocol](Session& session) {
        Message msg{ " User has Joined the Session " };
        server.SendAll(&msg);
    };
    server.on_message_ = [&server, &protocol](Session& session, const std::vector<uint8_t>& raw) {
        Message msg;
        protocol->Deserialise(raw, &msg);
        server.SendAll(&msg);
    };
    server.Run(socket);
}
