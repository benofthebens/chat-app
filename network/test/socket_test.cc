#include <gtest/gtest.h>
#include "network/socket.h"

class TcpSocketTest : public ::testing::Test {
protected:
    std::unique_ptr<NetworkSocket> tcp_socket_;
    void SetUp() override {
        tcp_socket_ = std::make_unique<NetworkSocket>();
    }
    void TearDown() override {
        tcp_socket_->Close();
    }
};

TEST_F(TcpSocketTest, ConstructorCreatesValidHandle) {
    EXPECT_NE(tcp_socket_->GetHandle(), INVALID_SOCKET);
}

TEST_F(TcpSocketTest, BindValidSocket) {
    Socket socket{ "127.0.0.1", 8080 };
    int result = tcp_socket_->Bind(socket);
    EXPECT_NE(result, SOCKET_ERROR);
}

TEST_F(TcpSocketTest, BindInvalidSocket) {
    Socket socket{ "INVALID", 8080 };
    int result = tcp_socket_->Bind(socket);
    EXPECT_EQ(result, SOCKET_ERROR);
}

TEST_F(TcpSocketTest, ListenValidSocket) {
    Socket socket{ "127.0.0.1", 8080 };
    const int bind_result = tcp_socket_->Bind(socket);
    EXPECT_NE(bind_result, SOCKET_ERROR);
    const int listen_result = tcp_socket_->Listen();
    EXPECT_NE(listen_result, SOCKET_ERROR);
}

TEST_F(TcpSocketTest, ListenInvalidSocket) {
    Socket socket{ "INVALID", 8080 };
    const int bind_result = tcp_socket_->Bind(socket);
    EXPECT_EQ(bind_result, SOCKET_ERROR);
    const int listen_result = tcp_socket_->Listen();
    EXPECT_EQ(listen_result, SOCKET_ERROR);
}

TEST_F(TcpSocketTest, ConnectValidSocket) {
    Socket socket{ "127.0.0.1", 8080 };

    const int bind_result = tcp_socket_->Bind(socket);
    EXPECT_NE(bind_result, SOCKET_ERROR);

    const int listen_result = tcp_socket_->Listen();
    EXPECT_NE(listen_result, SOCKET_ERROR);

    NetworkSocket client_socket;
    const int conn_result = client_socket.Connect(socket);
    EXPECT_NE(conn_result, SOCKET_ERROR);

    const auto client = tcp_socket_->Accept();
    EXPECT_NE(client, nullptr);
}

TEST_F(TcpSocketTest, ConnectInvalidSocket) {
    const Socket socket{ "INVALID", 8080 };

    const int bind_result = tcp_socket_->Bind(socket);
    EXPECT_EQ(bind_result, SOCKET_ERROR);

    const int listen_result = tcp_socket_->Listen();
    EXPECT_EQ(listen_result, SOCKET_ERROR);

    NetworkSocket client_socket;
    int conn_result = client_socket.Connect(socket);
    EXPECT_EQ(conn_result, SOCKET_ERROR);

    const auto client = tcp_socket_->Accept();
    EXPECT_EQ(client, nullptr);
}

TEST_F(TcpSocketTest, SendMessageOverConnection) {
    const Socket socket{ "127.0.0.1", 8080 };

    const int bind_result = tcp_socket_->Bind(socket);
    EXPECT_NE(bind_result, SOCKET_ERROR);

    const int listen_result = tcp_socket_->Listen();
    EXPECT_NE(listen_result, SOCKET_ERROR);

    NetworkSocket client_socket;
    const int conn_result = client_socket.Connect(socket);
    EXPECT_NE(conn_result, SOCKET_ERROR);

    const auto stream = tcp_socket_->Accept();
    EXPECT_NE(stream->GetHandle(), INVALID_SOCKET);

    const std::string message = "Hello World!";
    const int n = static_cast<int>(message.length());
    const int send_result = client_socket.Send(message.data(), n);
    EXPECT_EQ(send_result, message.length());

    std::string buffer(message.length(), '\0');
    const int receive_result = stream->Receive(buffer.data(), n);
    EXPECT_EQ(receive_result, message.length());
    EXPECT_EQ(buffer, message);
}
