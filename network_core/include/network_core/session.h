#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <functional>

#include "connection.h"
#include "protocol_handler.h"

template <typename TProtocol>
class Session {
private:
	std::shared_ptr<ClientConnection> conn_ = nullptr;
	ProtocolHandler<TProtocol> ph_;
public:
	explicit Session(const std::shared_ptr<ClientConnection>& conn)
        : conn_(conn) {}
	~Session() = default;
	void Run(std::function<void(Session&, const TProtocol&)> on_message) {
		while (conn_->IsConnected()) {
			std::vector<uint8_t> buffer(sizeof(TProtocol));
			int bytes_read = conn_->Receive(buffer.data(), buffer.size());

			if (bytes_read < 0) { break; }

			TProtocol data = ph_.Deserialise(reinterpret_cast<char*>(buffer.data()));
			on_message(*this, data);
		}
	}
 
	void Send(const TProtocol& data) {
		auto raw_data = ph_.Serialise(data);
		conn_->Send(raw_data.data(), raw_data.size());
	}
};
#endif