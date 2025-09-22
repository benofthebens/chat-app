#ifndef APPLICATION_CLIENT_H
#define APPLICATION_CLIENT_H

#include <functional>

#include "connection.h"
#include "protocol_handler.h"

template <typename TProtocol>
class ApplicationClient {
private:
	ClientConnection conn_;
	ProtocolHandler<TProtocol> ph_;
public:
	std::function<void()> on_connect_;
	std::function<void(TProtocol&)> on_message_;

	void Run(const Socket& socket) {
		conn_.Connect(socket);
		if (on_connect_) on_connect_();
		while (conn_.IsConnected()) {
			std::vector<uint8_t> buffer(sizeof(TProtocol));
			const int bytes_read = conn_.Receive(buffer.data(), buffer.size());
			if (bytes_read < 0) { break; }
			TProtocol data = ph_.Deserialise(reinterpret_cast<char*>(buffer.data()));
			if (on_message_) { on_message_(data); }
		}
	}
	void Send(const TProtocol& data) {
		std::vector<uint8_t> raw_data = ph_.Serialise(data);
		conn_.Send(raw_data.data(), raw_data.size());
	}
};

#endif