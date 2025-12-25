#ifndef APPLICATION_SERVER_H
#define APPLICATION_SERVER_H

#include <functional>
#include <map>
#include <mutex>
#include <atomic>
#include <thread>

#include "session.h"

template <typename TProtocol>
class ApplicationServer {
private:
	ServerConnection conn_;
	ProtocolHandler<TProtocol> ph_;
	std::map<SOCKET, std::shared_ptr<ClientConnection>> clients_ = {};
	std::atomic<bool> running_ = false;
public:
	std::function<void(Session<TProtocol>&)> on_connect_;
	std::function<void(Session<TProtocol>&, const TProtocol&)> on_message_;

	void Run(const Socket& socket) {
		conn_.Bind(socket);
		conn_.Listen();
		running_ = conn_.IsListening();
		while (running_) {
			auto conn = conn_.Accept();
			if (conn == nullptr) { continue; }
			auto client = std::make_shared<ClientConnection>(std::move(conn), true);
			auto session = std::make_shared<Session<TProtocol>>(client); 

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

	void SendAll(const TProtocol& data) {
		const std::vector<uint8_t> raw = ph_.Serialise(data);
	    for (auto& [handle, client] : clients_) {
			client->Send(raw.data(), raw.size());
	    }
	}

	void Send(const TProtocol& data) {
		std::vector<uint8_t> raw_data = ph_.Serialise(data);
		conn_.Send(raw_data.data(), raw_data.size());
	}
};

#endif