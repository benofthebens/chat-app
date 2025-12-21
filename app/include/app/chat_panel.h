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
	void AddMessage(const std::string& msg);
    void OnPaint(Engine::GraphicsContext& ctx) override;
};

#endif
