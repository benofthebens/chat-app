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


class ChatView : public Window<ChatView> {
private:
	std::vector<Message> messages_ = {};
	int scroll_position_ = 0;
public:
	ChatView();
	void PushMessage(Message* message) { messages_.push_back(*message); }
	LPCSTR class_name() override { return CHAT_VIEW_WC; }
private:
	void HandleScroll(WPARAM w_param, LPARAM l_param) override;
	void HandlePaint(WPARAM w_param, LPARAM l_param) override;
	void HandleSize(WPARAM w_param, LPARAM l_param) override;
}; 



#endif