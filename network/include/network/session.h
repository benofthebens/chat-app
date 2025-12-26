#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <functional>
#include <iostream>

#include "connection.h"
#include "protocol.h"

class Session {
private:
	std::shared_ptr<ClientConnection> conn_ = nullptr;
	std::shared_ptr<IProtocol> protocol_ = nullptr;
	std::atomic<bool> running_ = false;
public:
	explicit Session(const std::shared_ptr<ClientConnection>& conn, const std::shared_ptr<IProtocol>& protocol)
        : conn_(conn), protocol_(protocol) {}
	~Session() = default;
	void Run(std::function<void(Session&, const std::vector<uint8_t>&)> on_message) {
		running_ = conn_->IsConnected();
		while (running_) {
			std::vector<uint8_t> buffer(protocol_->MessageSize());
			const int n = static_cast<int>(buffer.size());
			const int bytes_read = conn_->Receive(buffer.data(), n);

			if (bytes_read < 0) { break; }

			if (on_message) { on_message(*this, buffer); }
		}
	}

	void Stop() {
		if (conn_) {
			conn_->Close();
		}
		running_ = false;
	}
 
	void Send(const void* data) {
		auto raw_data = protocol_->Serialise(data);
		conn_->Send(raw_data.data(), raw_data.size());
	}
};
#endif