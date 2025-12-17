#include "engine/application.h"

#include <cassert>

using namespace Engine;

Application::Application(const WindowProps& props) {
    instance = this;

    window_ = Window::Create(props);

    window_->SetEventCallback([this](Event& event) {
        OnEvent(event);
    });
}

void Application::Run() {
    OnInit();

    while (running_) {
        Window::PollEvents();

        for (const auto& layer : layer_stack_) {
            layer->OnUpdate();
        }

        for (const auto& layer : layer_stack_) {
            layer->OnRender();
        }
    }

    OnShutdown();
}

Application& Application::Get() {
    assert(instance);
    return *instance;
}

void Application::OnEvent(Event& event) {
    for (auto it = layer_stack_.rbegin(); it != layer_stack_.rend(); ++it) {
        (*it)->OnEvent(event);
        if (event.IsHandled()) {
            break;
        }
    }
}
