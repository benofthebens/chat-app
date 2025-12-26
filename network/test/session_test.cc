#include "network/session.h"

#include <gtest/gtest.h>

#include "network/connection.h"

struct TestProtocol {
    int id;
    char message[32];
};

TEST(SessionTest, Session_SendAndReceive) {
    constexpr WORD version_requested = MAKEWORD(2, 2);
    WSADATA wsa_data;
    WSAStartup(version_requested, &wsa_data);

    Socket socket{ "127.0.0.1", 8080 };

    auto server = std::make_shared<ServerConnection>();
    auto client = std::make_shared<ClientConnection>();
    std::shared_ptr<ClientConnection> conn = nullptr;

    server->Bind(socket);
    server->Listen();

    std::thread attach_thread([server, &conn]() {
        conn = std::make_shared<ClientConnection>(server->Accept(), true);
    });

    client->Connect(socket);
    attach_thread.join();
    ASSERT_NE(conn, nullptr);

    auto protocol = std::make_shared<PodProtocol<TestProtocol>>();

    std::string output;

    Session session(conn, protocol);
    std::thread session_thread([&output, &protocol, &session]() {
        session.Run([&output, &protocol](Session& session, const std::vector<uint8_t>& test) {
            TestProtocol tp;
            protocol->Deserialise(test, &tp);
            output = tp.message;
            session.Stop();
        });
    });

    TestProtocol message = { 1, "hello world" };
    auto raw = protocol->Serialise(&message);
    TestProtocol buffer;
    protocol->Deserialise(raw, &buffer);

    client->Send(raw.data(), sizeof(TestProtocol));

    session_thread.join();
    EXPECT_EQ(message.message, output);
}
