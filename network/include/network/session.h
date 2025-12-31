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
	void Run(std::function<void(Session&, const std::vector<uint8_t>&)> on_message);
	void Stop();
};
#endif