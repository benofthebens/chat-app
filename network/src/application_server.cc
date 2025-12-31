#include "network/application_server.h"

ApplicationServer::~ApplicationServer() {
    Stop(); 
}

void ApplicationServer::Run(const Socket& socket)  {
    conn_.Bind(socket);
    conn_.Listen();
    running_ = conn_.IsListening();
    while (running_) {
        auto conn = conn_.Accept();
        if (conn == nullptr) { continue; }
        auto client = std::make_shared<ClientConnection>(std::move(conn));
        auto session = std::make_shared<Session>(client, protocol_);

        clients_.push_back(client);

        if (on_connect_) { on_connect_(*session); }

        std::thread client_thread([session, this] {  session->Run(on_message_); });
        client_thread.detach();
    }
}

void ApplicationServer::Stop() {
    running_ = false;
    conn_.Close();
    for (auto& client : clients_) {
        client->Close();
    }
}

void ApplicationServer::SendAll(const void* data) {
	const std::vector<uint8_t> raw = protocol_->Serialise(data);
    const int n = static_cast<int>(raw.size());

    for (const auto& client : clients_) {
        client->Send(raw.data(), n);
    }
}

void ApplicationServer::Send(const void* data) {
    std::vector<uint8_t> raw = protocol_->Serialise(data);
    const int n = static_cast<int>(raw.size());
    conn_.Send(raw.data(), n);
}
