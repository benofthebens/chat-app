#ifndef APPLICATION_CLIENT_H
#define APPLICATION_CLIENT_H

#include <functional>
#include <mutex>

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
			const int n = static_cast<int>(buffer.size());
			const int bytes_read = conn_.Receive(buffer.data(), n);
			if (bytes_read < 0) { break; }
			TProtocol data = ph_.Deserialise(reinterpret_cast<char*>(buffer.data()));
			if (on_message_) { on_message_(data); }
		}
	}

	void Stop() {
		conn_.Close();
	}

	void Send(const TProtocol& data) {
		const std::vector<uint8_t> raw_data = ph_.Serialise(data);
		const int n = static_cast<int>(raw_data.size());
		conn_.Send(raw_data.data(), n);
	}
};

#endif