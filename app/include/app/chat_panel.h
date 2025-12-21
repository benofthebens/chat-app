#ifndef CHAT_VIEW_H
#define CHAT_VIEW_H

#include "engine/panel.h"

class ChatPanel final : public Engine::Panel {
private:
	std::vector<std::string> messages_;
public:
	explicit ChatPanel(const Engine::PanelProps& props)
        : Panel(props) {}
	~ChatPanel() override = default;
    void AddMessage(const std::string& msg) {
        messages_.push_back(msg);
        InvalidateRect(GetHwnd(), nullptr, TRUE);
    }
    void OnPaint(Engine::GraphicsContext& ctx) override;
};

inline void ChatPanel::OnPaint(Engine::GraphicsContext& ctx) {
    int y = 10;
    for (const auto& msg : messages_) {
        ctx.OutText(msg, { 10, y });
        y += 20;
    }
}

#endif
