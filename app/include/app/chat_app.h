#ifndef CHAT_APP_H
#define CHAT_APP_H

#include "engine/application.h"
#include "engine/panel.h"
#include "engine/widgets/text_input.h"

class ChatAppLayer : public Engine::Layer {
private:
    Engine::Panel* panel_;
public:
    ChatAppLayer() = default;
    ~ChatAppLayer() override = default;
    void OnEvent(Engine::Event& event) override;
    void OnRender(Engine::GraphicsContext& ctx) override;
    void OnAttach(Engine::Window& window) override;
};

#endif