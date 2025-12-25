#include "network_core/session.h"

#include <gtest/gtest.h>

#include "network_core/connection.h"

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

    std::string output;

    Session<TestProtocol> session(conn);
    std::thread session_thread([&session, &output]() {
        session.Run([&output](Session<TestProtocol>& session, const TestProtocol& test) {
            output = test.message;
            session.Stop();
        });
    });

    TestProtocol message = { 1, "hello world" };
    client->Send(&message, sizeof(TestProtocol));

    session_thread.join();
    EXPECT_EQ(message.message, output);
}
