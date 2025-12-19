#include "engine/window.h"

#include "engine/widgets/button.h"

using namespace Engine;

std::shared_ptr<Window> Window::Create(const WindowProps& props) {
    return std::make_shared<Window>(props);
}

void Window::OnInit(const WindowProps& props) {
    label_ = props.label.c_str();
    width_ = props.width;
    height_ = props.height;

    hinstance_ = GetModuleHandle(nullptr);

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hinstance_;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.lpszClassName = kEngineWindow;

    if (!RegisterClassEx(&wc)) {
        return;
    }

    RECT rect = { 0, 0, static_cast<LONG>(width_), static_cast<LONG>(height_) };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    hwnd_ = CreateWindowEx(
        props.ex_style,
        kEngineWindow,
        label_,
        props.style,
        props.x, props.y,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        hinstance_,
        this  
    );

    ShowWindow(hwnd_, SW_SHOW);
}

void Window::PollEvents() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Window::RaiseEvent(Event& event) const {
    if (callback_) {
        callback_(event);
    }
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
    Window* instance = nullptr;

    if (msg == WM_NCCREATE) {
        const auto* create_struct = reinterpret_cast<LPCREATESTRUCT>(l_param);
        instance = static_cast<Window*>(create_struct->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));
        instance->hwnd_ = hwnd;
    }
    else {
        instance = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    if (instance) {
        return instance->HandleMessage(msg, w_param, l_param);
    }

    return DefWindowProc(hwnd, msg, w_param, l_param);
}

LRESULT Window::HandleMessage(UINT msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
    case WM_DESTROY: {
        WindowClosedEvent event;
        RaiseEvent(event);
        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd_, &ps);
        RECT rect;
        GetClientRect(hwnd_, &rect);
        FillRect(hdc, &rect, nullptr);

        GraphicsContext ctx(hdc);
        WindowPaintEvent event(ctx);
        RaiseEvent(event);

        EndPaint(hwnd_, &ps);
        return 0;
    }
    case WM_SIZE: {
        width_ = LOWORD(l_param);
        height_ = HIWORD(l_param);

        WindowResizeEvent event(width_, height_);

        RaiseEvent(event);
        return 0;
    }
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN: {
        return 0;
    }
    case WM_KEYUP:
    case WM_SYSKEYUP: {
        return 0;
    }
    }
    return DefWindowProc(hwnd_, msg, w_param, l_param);
}
