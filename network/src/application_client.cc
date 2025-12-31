#include "network/application_client.h"

ApplicationClient::~ApplicationClient() {
    Stop(); 
}

void ApplicationClient::Run(const Socket& socket) {
    conn_.Connect(socket);
    if (on_connect_) on_connect_();
    while (conn_.IsConnected()) {
        std::vector<uint8_t> buffer(protocol_->MessageSize());
        const int n = static_cast<int>(buffer.size());
        const int bytes_read = conn_.Receive(buffer.data(), n);
        if (bytes_read < 0) { break; }
        if (on_message_) { on_message_(buffer); }
    }
}

void ApplicationClient::Stop() {
    conn_.Close();
}

void ApplicationClient::Send(const void* data) {
    const std::vector<uint8_t> raw_data = protocol_->Serialise(data);
    const int n = static_cast<int>(raw_data.size());
    conn_.Send(raw_data.data(), n);
}
