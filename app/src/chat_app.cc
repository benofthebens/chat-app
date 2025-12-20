#include "app/chat_app.h"

#include "app/network_events.h"
#include "engine/window_props.h"
#include "engine/widgets/button.h"
#include "engine/props_builder.h"

void ChatAppLayer::OnEvent(Engine::Event& event) {
    Engine::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<MessageReceiveEvent>([this](MessageReceiveEvent& e) {
        HWND hwnd = Application::Get().GetWindow()->GetHwnd();
        MessageBox(hwnd, e.GetMsg().data, "Message", MB_OK);
        return false;  
    });
}

void ChatAppLayer::OnRender(Engine::GraphicsContext& ctx) {}

void ChatAppLayer::OnAttach(Engine::Window& window) {
    Engine::PanelProps props = Engine::PropsBuilder<Engine::PanelProps>()
        .Parent(window.GetHwnd())
        .Position(0,0)
        .Size(window.GetWidth(), window.GetHeight())
        .Style(WS_BORDER)
        .Build();
    props.class_name = "MAIN_PANEL";

    panel_ = new Engine::Panel(props);
    Engine::TextInputProps tiprops= Engine::PropsBuilder<TextInputProps>()
        .Parent(window.GetHwnd())
        .Position(50, 50)
        .Size(100, 50)
        .Style(WS_BORDER)
        .Build();

    auto input = panel_->AddChild<TextInput>(tiprops);

    Engine::ButtonProps bps = Engine::PropsBuilder<Engine::ButtonProps>()
        .Position(0, 0)
        .Size(50, 50)
        .Label("Hello")
        .Build();

    bps.on_click = [input]() {
        Message msg{};
        strcpy_s(msg.data, input->GetText().c_str());

        MessageSendEvent event(msg);
        Application::Get().RaiseEvent(event);
    };
    panel_->AddChild<Engine::Button>(bps);
}
