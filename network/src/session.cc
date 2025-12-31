#include "network/session.h"

void Session::Run(std::function<void(Session&, const std::vector<uint8_t>&)> on_message) {
	running_ = conn_->IsConnected();
    while (running_) {
        std::vector<uint8_t> buffer(protocol_->MessageSize());
        const int n = static_cast<int>(buffer.size());
        const int bytes_read = conn_->Receive(buffer.data(), n);

        if (bytes_read < 0) { break; }

        if (on_message) { on_message(*this, buffer); }
    }
}

void Session::Stop() {
    if (conn_) {
        conn_->Close();
    }
    running_ = false;
}
