#ifndef CHAT_APP_H
#define CHAT_APP_H

#include "engine/application.h"

class ChatAppLayer : public Engine::Layer {
public:
    ChatAppLayer() = default;
    ~ChatAppLayer() override = default;
protected:
    void OnEvent(Engine::Event& event) override;
    void OnUpdate() override;
    void OnRender(Engine::GraphicsContext& ctx) override;
};

#endif