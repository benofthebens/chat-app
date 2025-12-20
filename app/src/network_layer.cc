#include "app/network_layer.h"

#include <thread>
#include "app/network_events.h"
#include "engine/application.h"

void NetworkLayer::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<MessageSendEvent>([this](const MessageSendEvent& e) {
        client_.Send(e.GetMsg());
        return true;
    });
}

void NetworkLayer::OnAttach(Engine::Window& window) {
    client_.on_message_ = [this](const Message& msg) {
        std::lock_guard lock(mutex_);
        pending_messages_.push(msg);
    };
    client_.on_connect_ = []() {
        OutputDebugString("Client Connected");
    };
    Socket socket = { "127.0.0.1", 8080 };
    network_thread_ = std::thread(
        &ApplicationClient<Message>::Run, &client_, socket);
    network_thread_.detach();
}

void NetworkLayer::OnUpdate() {
    std::lock_guard<std::mutex> lock(mutex_);
    while (!pending_messages_.empty()) {
        MessageReceiveEvent event(pending_messages_.front());
        Engine::Application::Get().RaiseEvent(event);
        pending_messages_.pop();
    }
}

