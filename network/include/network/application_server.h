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
	std::map<SOCKET, std::shared_ptr<ClientConnection>> clients_ = {};
	std::atomic<bool> running_ = false;
public:
	std::function<void(Session&)> on_connect_;
	std::function<void(Session&, const std::vector<uint8_t>&)> on_message_;

	explicit ApplicationServer(const std::shared_ptr<IProtocol>& protocol) : protocol_(protocol) {}
	~ApplicationServer() { Stop(); }

	void Run(const Socket& socket) {
		conn_.Bind(socket);
		conn_.Listen();
		running_ = conn_.IsListening();
		while (running_) {
			auto conn = conn_.Accept();
			if (conn == nullptr) { continue; }
			auto client = std::make_shared<ClientConnection>(std::move(conn), true);
			auto session = std::make_shared<Session>(client, protocol_); 

			clients_.insert({ client->Handle(), client });

			if (on_connect_) { on_connect_(*session); }

			std::thread client_thread([session, this]() {  
				session->Run(on_message_);
			});
			client_thread.detach();
		}
	}

	void Stop() {
		running_ = false;
		conn_.Close();
		for (auto& [socket, client] :clients_) {
			closesocket(socket);
			client->Close();
		}
	}

	void SendAll(const void* data) {
		const std::vector<uint8_t> raw = protocol_->Serialise(data);
		const int n = static_cast<int>(raw.size());
		for (auto& [handle, client] : clients_) {
			client->Send(raw.data(), n);
		}
	}

	void Send(const void* data) {
		std::vector<uint8_t> raw = protocol_->Serialise(data);
		const int n = static_cast<int>(raw.size());
		conn_.Send(raw.data(), n);
	}
};

#endif