#include "app/chat_app.h"

void ChatAppLayer::OnEvent(Engine::Event& event) {
    Engine::EventDispatcher dispatcher(event);
}

void ChatAppLayer::OnUpdate() {
    Layer::OnUpdate();
}

void ChatAppLayer::OnRender(Engine::GraphicsContext& ctx) {
    Engine::Point p1;
    p1.x = 60;
    p1.y = 60;

    Engine::Point p2;
    p2.x = 120;
    p2.y = 400;

    auto blue = Engine::Colour::Blue();
    ctx.DrawSolidLine(p1, p2, blue, 50);
}
