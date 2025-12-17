#include "app/chat_app.h"

void ChatAppLayer::OnEvent(Engine::Event& event) {
    Engine::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Engine::WindowPaintEvent>([this](Engine::WindowPaintEvent& e)-> bool {
        HWND hwnd = Engine::Application::Get().GetWindow()->GetHwnd();
        RECT client;
        GetClientRect(hwnd, &client);

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect = { 50,100,200,50 };
        FillRect(hdc, &client, nullptr);
        FillRect(hdc, &rect, reinterpret_cast<HBRUSH>(DKGRAY_BRUSH));
        
        EndPaint(hwnd, &ps);
        return true;
    });
}

void ChatAppLayer::OnUpdate() {
    Layer::OnUpdate();
}

void ChatAppLayer::OnRender() {

}
