#ifndef APPLICATION_SERVER_H
#define APPLICATION_SERVER_H

#include <functional>
#include <map>
#include <mutex>
#include <atomic>
#include <thread>

#include "session.h"

class ApplicationServer {
private:
	ServerConnection conn_;
	std::shared_ptr<IProtocol> protocol_ = nullptr;
	std::vector<std::shared_ptr<ClientConnection>> clients_ {};
	std::atomic<bool> running_ = false;
public:
	std::function<void(Session&)> on_connect_;
	std::function<void(Session&, const std::vector<uint8_t>&)> on_message_;
	explicit ApplicationServer(const std::shared_ptr<IProtocol>& protocol)
        : protocol_(protocol) {}
	~ApplicationServer();
	void Run(const Socket& socket);
	void Stop();
	void SendAll(const void* data);
	void Send(const void* data); 
};

#endif