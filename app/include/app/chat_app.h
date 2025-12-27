#ifndef CHAT_APP_H
#define CHAT_APP_H

#include <cassert>

#include "network_events.h"
#include "engine/application.h"
#include "engine/panel.h"
#include "engine/widgets/button.h"
#include "engine/widgets/text_input.h"
#include "app/chat_panel.h"
class ChatAppLayer : public Layer {
private:
    ChatPanel* chat_view_ = nullptr;
    TextInput* input_ = nullptr;
    Button* button_ = nullptr;
    Panel* panel_ = nullptr;
public:
    ChatAppLayer();
    ~ChatAppLayer() override;
    void OnEvent(Event& event) override;
    void OnRender(GraphicsContext& ctx) override;
    void OnAttach(Window& window) override;
};


#endif