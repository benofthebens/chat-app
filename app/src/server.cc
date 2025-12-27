
#include <iostream>

#include "app/network_events.h"
#include "network/application_server.h"

int main() {
    auto protocol = std::make_shared<PodProtocol<Message>>();
    ApplicationServer server(protocol);

    Socket socket = { "127.0.0.1", 8080 };
    server.on_connect_ = [&server, &protocol](Session& session) {
        Message msg;
        msg.user.id = -1;
        strcpy_s(msg.user.name,"");
        strcpy_s(msg.data,"User has Joined the session");
        server.SendAll(&msg);
    };
    server.on_message_ = [&server, &protocol](Session& session, const std::vector<uint8_t>& raw) {
        Message msg;
        protocol->Deserialise(raw, &msg);
        server.SendAll(&msg);
    };
    server.Run(socket);
}
