#ifndef APPLICATION_CLIENT_H
#define APPLICATION_CLIENT_H

#include <functional>
#include <mutex>

#include "connection.h"
#include "protocol.h"

class ApplicationClient {
private:
	ClientConnection conn_;
	std::shared_ptr<IProtocol> protocol_;
public:
	std::function<void()> on_connect_;
	std::function<void(const std::vector<uint8_t>&)> on_message_;

	ApplicationClient(const std::shared_ptr<IProtocol>& protocol)
		: protocol_(protocol) {}
	~ApplicationClient() { Stop(); }
	void Run(const Socket& socket) {
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

	void Stop() {
		conn_.Close();
	}

	void Send(const void* data) {
		const std::vector<uint8_t> raw_data = protocol_->Serialise(data);
		const int n = static_cast<int>(raw_data.size());
		conn_.Send(raw_data.data(), n);
	}
};

#endif