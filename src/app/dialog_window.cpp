#include "dialog_window.h"
#include <string>

DialogWindow::~DialogWindow() = default;
void DialogWindow::set_target_command(const unsigned int target_command) {
    target_command_ = target_command;
}
LPCSTR DialogWindow::class_name() const {
    return "dialog";
}

int DialogWindow::handle_command(WPARAM w_param, LPARAM l_param) {
    switch (LOWORD(w_param)) {
    case START_SERVER: {
        Socket* socket = new Socket();

        char host_buffer[256]; 
        char port_buffer[256];

        GetWindowText(host_input_window_, host_buffer, 256);
        GetWindowText(port_input_window_, port_buffer, 256);
        
        int port = std::stoi(port_buffer);

        socket->host = host_buffer;
        socket->port = port;

        PostMessage(
            parent_window_handle_,
            WM_COMMAND,
            target_command_,
            (LPARAM)socket
        );
        PostMessage(window_handle_, WM_DESTROY, 0, 0);

        return 0;
    }
    default:
        return 0;
    }
}

LRESULT DialogWindow::handle_message(UINT msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
    case WM_DESTROY: {
        DestroyWindow(window_handle_);
        return 0;
    }
    case WM_COMMAND:
        return handle_command(w_param, l_param);
    case WM_CREATE: {
        RECT rect;
        GetClientRect(window_handle_, &rect);

        const unsigned int width = rect.right - rect.left;
        const unsigned int height = rect.bottom - rect.top;

        host_input_window_ = CreateWindowW(
            L"edit", L"127.0.0.1", WS_VISIBLE | WS_CHILD | WS_BORDER, 50, 25, width - 100, 20, window_handle_, NULL, NULL, NULL
        );

        port_input_window_ = CreateWindowW(
            L"edit", L"8080", WS_VISIBLE | WS_CHILD | WS_BORDER, 50, 50, width - 100, 20, window_handle_, NULL, NULL, NULL
        );

        connect_button_window_ = CreateWindowW(
            L"button", L"connect", WS_VISIBLE | WS_CHILD, 50, 75, width - 100, 20, window_handle_, (HMENU)START_SERVER, NULL, NULL
        );

        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT paint;
        HDC hdc = BeginPaint(window_handle_, &paint);

        FillRect(hdc, &paint.rcPaint, (HBRUSH)(COLOR_WINDOW));

        EndPaint(window_handle_, &paint);

        return 0;
    }
    default:
        return DefWindowProc(window_handle_, msg, w_param, l_param);
    }
}