#ifndef CHAT_VIEW_H 
#define CHAT_VIEW_H 

#include <vector>
#include <Windows.h>

#include "win_widgets/window.h"

#define CHAT_VIEW_WC "CHAT_VIEW"

class ChatView final : public Window<ChatView> {
private:
public:
	ChatView() = default;
	LPCSTR class_name() override { return CHAT_VIEW_WC; }
private:
	void HandleScroll(WPARAM w_param, LPARAM l_param) override;
	void HandlePaint(WPARAM w_param, LPARAM l_param) override;
	void HandleSize(WPARAM w_param, LPARAM l_param) override;
}; 

#endif