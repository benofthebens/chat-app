#ifndef CHAT_APP_H
#define CHAT_APP_H

#include "engine/application.h"
#include "engine/panel.h"
#include "engine/widgets/button.h"
#include "engine/widgets/text_input.h"
#include "app/chat_panel.h"

class ChatAppLayer : public Engine::Layer {
private:
    std::vector<std::string> messages_;
    Engine::Panel* panel_ = nullptr;
    ChatPanel* chat_view_ = nullptr;
    Engine::TextInput* input_ = nullptr;
    Engine::Button* button_ = nullptr;
public:
    ChatAppLayer() = default;
    ~ChatAppLayer() override;
    void OnEvent(Engine::Event& event) override;
    void OnRender(Engine::GraphicsContext& ctx) override;
    void OnAttach(Engine::Window& window) override;
};

#endif