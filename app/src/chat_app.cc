#include "app/chat_app.h"

#include "app/network_events.h"
#include "engine/window_props.h"
#include "engine/widgets/button.h"
#include "engine/props_builder.h"

ChatAppLayer::~ChatAppLayer() {
    delete panel_;
    delete chat_view_;
    delete input_;
    delete button_;
}

void ChatAppLayer::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<MessageReceiveEvent>([this](MessageReceiveEvent& e) {
        chat_view_->AddMessage(e.GetMsg().data);
        return false;  
    });
}

void ChatAppLayer::OnRender(GraphicsContext& ctx) {}

void ChatAppLayer::OnAttach(Window& window) {

    PanelProps main_panel_props = PropsBuilder<PanelProps>()
        .Parent(window.GetHwnd())
        .Position(0,0)
        .Size(window.GetWidth(), window.GetHeight())
        .Build();
    main_panel_props.class_name = "MAIN_PANEL";
    panel_ = new Panel(main_panel_props);

    PanelProps chat_view_props = PropsBuilder<PanelProps>()
        .Position(300, 10)
        .Size(600, 500)
        .Style(WS_BORDER | WS_VSCROLL)
        .Build();
    chat_view_props.class_name = "CHAT_VIEW";

    TextInputProps input_props = PropsBuilder<TextInputProps>()
        .Position(chat_view_props.x, chat_view_props.y + 500)
        .Size(chat_view_props.width - 50, 50)
        .Style(WS_BORDER | ES_MULTILINE | WS_VSCROLL)
        .Build();

    ButtonProps button_props = PropsBuilder<ButtonProps>()
        .Position(input_props.x + input_props.width, input_props.y)
        .Size(50, 50)
        .Label("Send")
        .Build();
    button_props.on_click = [this]{
        Message msg{};
        strcpy_s(msg.data, input_->GetText().c_str());
        MessageSendEvent event(msg);
        Application::Get().RaiseEvent(event);
    };

    chat_view_ = panel_->AddChild<ChatPanel>(chat_view_props);
    input_ = panel_->AddChild<TextInput>(input_props);
    button_ = panel_->AddChild<Button>(button_props);
}
