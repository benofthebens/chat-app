#ifndef CHAT_VIEW_H
#define CHAT_VIEW_H

#include "network_events.h"
#include "engine/panel.h"

class ChatPanel final : public Engine::Panel {
private:
	std::vector<Message> messages_;
public:
	explicit ChatPanel(const Engine::PanelProps& props)
        : Panel(props) {}
	~ChatPanel() override = default;
	void AddMessage(const Message& msg);
    void OnPaint(Engine::GraphicsContext& ctx) override;
};

#endif
