#include "main_window.h"
#include <cwchar>

int MainWindow::handle_command(WPARAM w_param, LPARAM l_param) {
    switch (LOWORD(w_param)) {
    case START_SERVER: {
        if (server_) return 0;
        server_ = new Server("127.0.0.1", 8080);
        std::thread server_thread(&Server::start, server_);
        server_thread.detach();
        MessageBox(window_handle_, "Started Server on port 8080", "", MB_OK);
        return 0;
    }
    case START_CLIENT: {
        if (client_) return 0;
        client_ = new Client("127.0.0.1", 8080);
        client_->set_message_handler([this](const char x[]) {
            PostMessage(
                window_handle_,
                WM_COMMAND,
                DISPLAY_TEXT,
                reinterpret_cast<LPARAM>(x)
            );
        });
        std::thread client_thread(&Client::start, client_);
        client_thread.detach();
        MessageBox(window_handle_, "Connected to Server", "", MB_OK);
        return 0;
    }
    case SEND_MESSAGE: {
        char buffer[256];
        GetWindowText(text_window_, buffer, sizeof(buffer));
        client_->send_data(buffer);
        SetWindowText(text_window_, "");
        return 0;
    }
    case DISPLAY_TEXT: {
        char* msg = reinterpret_cast<char*>(l_param);
        
        display_text_ += "> ";
        display_text_ += msg;
        display_text_ += "\r\n ";

        SetWindowText(display_window_, display_text_.c_str());
        return 0;
    }
    default:
        return 0;
    }
}

LPCSTR MainWindow::class_name() const {
    return "main";
}

LRESULT MainWindow::handle_message(UINT msg, WPARAM w_param, LPARAM l_param) {
     // Handles the messages
    switch (msg) {
     case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_COMMAND:
        return handle_command(w_param, l_param);
    case WM_CREATE: {
        RECT rect;
        GetClientRect(window_handle_, &rect);

        int width = rect.right - rect.left;
        int height = rect.top - rect.bottom;
        
        CreateWindowW(
            L"button", L"Send",
            WS_VISIBLE | WS_CHILD,
            50, 400, 100, 50,
            window_handle_,
            (HMENU)SEND_MESSAGE, NULL,NULL
        );

        text_window_ = CreateWindowW(
            L"edit",L"",
            WS_VISIBLE | WS_CHILD,
            50, 300, width - 100, 50,
            window_handle_,
            NULL, NULL, NULL
        );

        display_window_ = CreateWindowW(
            L"edit",L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL |
            ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
            50, 50, width - 100, 200,
            window_handle_,
            NULL, NULL, NULL 
        );

        CreateWindowW(
            L"button", L"Start Server",
            WS_VISIBLE | WS_CHILD,
            width - 150, 400, 100, 50,
            window_handle_,
            (HMENU)START_SERVER, NULL, NULL
        );

        CreateWindowW(
            L"button", L"Connect",
            WS_VISIBLE | WS_CHILD,
            width - 250, 400, 100, 50,
            window_handle_,
            (HMENU)START_CLIENT, NULL, NULL
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
