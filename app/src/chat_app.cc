#include "app/chat_app.h"
#include "engine/window_props.h"
#include "engine/widgets/button.h"
#include "engine/props_builder.h"

void ChatAppLayer::OnEvent(Engine::Event& event) {}

void ChatAppLayer::OnRender(Engine::GraphicsContext& ctx) {}

void ChatAppLayer::OnAttach(Engine::Window& window) {
    Engine::PanelProps props = Engine::PropsBuilder<Engine::PanelProps>()
        .Parent(window.GetHwnd())
        .Position(0,0)
        .Size(100, 100)
        .Style(WS_BORDER)
        .Build();
    props.class_name = "MAIN_PANEL";

    panel_ = new Engine::Panel(props);

    Engine::ButtonProps bps = Engine::PropsBuilder<Engine::ButtonProps>()
        .Position(0, 0)
        .Size(200, 200)
        .Label("Hello")
        .Build();
    bps.on_click = [window]() {
        MessageBox(window.GetHwnd(), "Hello", "Hello", MB_OK);
    };
    panel_->AddChild<Engine::Button>(bps);

}
