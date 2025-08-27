#ifndef CHAT_VIEW_H 
#define CHAT_VIEW_H 

#include "client.h"
#include "server.h"
#include "message.h"
#include "win_widgets/window.h"
#include <Windows.h>
#include "chat_app.h"
#include <mutex>
#include <vector>

#define CHAT_VIEW_WC "CHAT_VIEW"

class ChatView final : public Window<ChatView> {
private:
	std::vector<Message> messages_ = {};
public:
	ChatView() = default;
	void AddMessage(Message* message);
	LPCSTR class_name() override { return CHAT_VIEW_WC; }
private:
	void HandleScroll(WPARAM w_param, LPARAM l_param) override;
	void HandlePaint(WPARAM w_param, LPARAM l_param) override;
	void HandleSize(WPARAM w_param, LPARAM l_param) override;
}; 



#endif