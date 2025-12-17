#include "engine/window.h"

using namespace Engine;

std::shared_ptr<Window> Window::Create(const WindowProps& props) {
    return std::make_shared<Window>(props);
}

void Window::OnInit(const WindowProps& props) {
    data_.title = props.title;
    data_.width = props.width;
    data_.height = props.height;

    hinstance_ = GetModuleHandle(nullptr);

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hinstance_;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.lpszClassName = kClassName;

    if (!RegisterClassEx(&wc)) {
        return;
    }

    RECT rect = { 0, 0, static_cast<LONG>(data_.width), static_cast<LONG>(data_.height) };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    hwnd_ = CreateWindowEx(
        0,
        kClassName,
        data_.title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
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

void Window::RaiseEvent(Event& event) {
    if (data_.callback) {
        data_.callback(event);
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
        if (data_.callback) {
            WindowClosedEvent event;
            data_.callback(event);
        }
        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd_, &ps);
        RECT rect;
        GetClientRect(hwnd_, &rect);
        FillRect(hdc, &rect, nullptr);
        GraphicsContext ctx(hdc);

        if (data_.callback) {
            WindowPaintEvent event(ctx);
            data_.callback(event);
        }
        EndPaint(hwnd_, &ps);
        return 0;
    }
    case WM_SIZE: {
        data_.width = LOWORD(l_param);
        data_.height = HIWORD(l_param);

        if (data_.callback) {
            WindowResizeEvent event(data_.width, data_.height);
            data_.callback(event);
        }

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
