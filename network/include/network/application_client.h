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
	~ApplicationClient();
	void Run(const Socket& socket);
	void Stop();
	void Send(const void* data);
};

#endif