#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <functional>
#include <iostream>

#include "connection.h"
#include "protocol_handler.h"

template <typename TProtocol>
class Session {
private:
	std::shared_ptr<ClientConnection> conn_ = nullptr;
	ProtocolHandler<TProtocol> ph_;
	std::atomic<bool> running_ = false;
public:
	explicit Session(const std::shared_ptr<ClientConnection>& conn)
        : conn_(conn) {}
	~Session() = default;
	void Run(std::function<void(Session&, const TProtocol&)> on_message) {
		running_ = conn_->IsConnected();
		while (running_) {
			std::vector<uint8_t> buffer(sizeof(TProtocol));
			const int n = static_cast<int>(buffer.size());
			const int bytes_read = conn_->Receive(buffer.data(), n);

			if (bytes_read < 0) { break; }

			TProtocol data = ph_.Deserialise(reinterpret_cast<char*>(buffer.data()));
			if (on_message) { on_message(*this, data); }
		}
	}

	void Stop() {
		if (conn_) {
			conn_->Close();
		}
		running_ = false;
	}
 
	void Send(const TProtocol& data) {
		auto raw_data = ph_.Serialise(data);
		conn_->Send(raw_data.data(), raw_data.size());
	}
};
#endif