#include "network/connection.h"

#include <gtest/gtest.h>

class ConnectionTest : public ::testing::Test {
protected:
    std::unique_ptr<ServerConnection> server_;
    std::unique_ptr<ClientConnection> client_;

    void SetUp() override {
        constexpr WORD version_requested = MAKEWORD(2, 2); // gets the version requested
        WSADATA wsa_data;
        WSAStartup(version_requested, &wsa_data);

        server_ = std::make_unique<ServerConnection>();
        client_ = std::make_unique<ClientConnection>();
    }

    void TearDown() override {
        client_->Close();
        server_->Close();
        WSACleanup();
    }
};

TEST_F(ConnectionTest, Server_BindValidSocket) {
    const Socket socket = { "127.0.0.1", 8080 };
    const int result = server_->Bind(socket);
    EXPECT_NE(result, -1);
}

TEST_F(ConnectionTest, Server_BindDuplicateSocket) {
    const Socket socket = { "127.0.0.1", 8080 };
    const int result1 = server_->Bind(socket);
    EXPECT_NE(result1, -1);
    const int result2 = server_->Bind(socket);
    EXPECT_EQ(result2, -1);
}

TEST_F(ConnectionTest, Server_BindInvalidSocket) {
    const Socket socket = { "INVALID", 8080 };
    const int result = server_->Bind(socket);
    EXPECT_EQ(result, -1);
}

TEST_F(ConnectionTest, Server_ListenValidSocket) {
    const Socket socket = { "127.0.0.1", 8080 };

    const int bind_result = server_->Bind(socket);
    EXPECT_NE(bind_result, -1);

    const int listen_result = server_->Listen();
    EXPECT_NE(listen_result, SOCKET_ERROR);
    EXPECT_TRUE(server_->IsListening());
}

TEST_F(ConnectionTest, Server_ListenInvalidSocket) {
    const Socket socket = { "INVALID", 8080 };

    const int bind_result = server_->Bind(socket);
    EXPECT_EQ(bind_result, -1);

    const int listen_result = server_->Listen();
    EXPECT_EQ(listen_result, SOCKET_ERROR);
    EXPECT_FALSE(server_->IsListening());
}

TEST_F(ConnectionTest, Server_ListenUnboundSocket) {
    const Socket socket = { "127.0.0.1", 8080 };

    const int listen_result = server_->Listen();
    EXPECT_EQ(listen_result, SOCKET_NOT_BOUND);
    EXPECT_FALSE(server_->IsListening());
}

TEST_F(ConnectionTest, Client_ConnectValidSocket) {
    const Socket socket = { "127.0.0.1", 8080 };

    const int bind_result = server_->Bind(socket);
    EXPECT_NE(bind_result, -1);

    const int listen_result = server_->Listen();
    EXPECT_NE(listen_result, SOCKET_ERROR);
    EXPECT_TRUE(server_->IsListening());

    const int conn_result = client_->Connect(socket);
    EXPECT_NE(conn_result, SOCKET_ERROR);

    const auto client = server_->Accept();
    EXPECT_NE(client, nullptr);
    EXPECT_TRUE(client_->IsConnected());
}

TEST_F(ConnectionTest, Client_ConnectInvalidSocket) {
    const Socket socket = { "INVALID", 8080 };

    const int bind_result = server_->Bind(socket);
    EXPECT_EQ(bind_result, -1);

    const int listen_result = server_->Listen();
    EXPECT_EQ(listen_result, SOCKET_ERROR);
    EXPECT_FALSE(server_->IsListening());

    const int conn_result = client_->Connect(socket);
    EXPECT_EQ(conn_result, SOCKET_ERROR);

    const auto client = server_->Accept();
    EXPECT_EQ(client, nullptr);
    EXPECT_FALSE(client_->IsConnected());
}

TEST_F(ConnectionTest, Client_SendOnValidConnection) {
    const Socket socket = { "127.0.0.1", 8080 };

    const int bind_result = server_->Bind(socket);
    EXPECT_NE(bind_result, -1);

    const int listen_result = server_->Listen();
    EXPECT_NE(listen_result, SOCKET_ERROR);
    EXPECT_TRUE(server_->IsListening());

    const int conn_result = client_->Connect(socket);
    EXPECT_NE(conn_result, SOCKET_ERROR);

    const auto client = server_->Accept();
    EXPECT_NE(client, nullptr);
    EXPECT_TRUE(client_->IsConnected());

    const std::string message = "Hello World";
    const int n = static_cast<int>(message.length());

    const int send_result = client_->Send(message.data(), n);
    ASSERT_EQ(send_result, n);

    std::string buffer(message.length(), '\0');
    const int receive_result = client->Receive(buffer.data(), n);
    EXPECT_EQ(receive_result, n);
    EXPECT_EQ(buffer, message);
}

TEST_F(ConnectionTest, Client_SendInvalidConnetion) {
    const auto* test_data = "TEST";
    const int n = static_cast<int>(strlen(test_data));

    const int result = client_->Send(test_data, n);
    EXPECT_EQ(result, SOCKET_NOT_BOUND);
}

