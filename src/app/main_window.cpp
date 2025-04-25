#include "main_window.h"
#include <cwchar>
#include <mutex>
#include <thread>

MainWindow::~MainWindow() {
    delete client_;
    delete server_;
}

int MainWindow::handle_command(WPARAM w_param, LPARAM l_param) {
    switch (LOWORD(w_param)) {
    case START_SERVER: {
        if (server_) {
            MessageBox(window_handle_, "Server is already running", "", MB_OK );
            return 0;
        }

        server_ = new Server("127.0.0.1", 8080);
        std::thread server_thread(&Server::start, server_);
        server_thread.detach();

        MessageBox(window_handle_, "Started Server on port 8080", "", MB_OK | MB_ICONINFORMATION);
        return 0;
    }
    case START_CLIENT: {
        if (client_) {
            MessageBox(window_handle_, "Client is already running", "", MB_OK);
            return 0;
        }

        client_ = new Client("127.0.0.1", 8080);
        client_->set_message_receiver_handler([this](const char* x) {
            PostMessage(
                window_handle_,
                WM_COMMAND,
                DISPLAY_TEXT,
                reinterpret_cast<LPARAM>(x)
            );
        });
        client_->set_message_sender_handler([this](char* buffer){
            // Thread safety 
            std::lock_guard<std::mutex> lock_guard(text_mutex_);

            GetWindowText(text_window_, buffer, 256);
            SetWindowText(text_window_, "");

            return buffer;
        });
        client_->start();

        MessageBox(window_handle_, "Connected to Server", "", MB_OK);
        return 0;
    }
    case SEND_MESSAGE: {
        client_->send_request();
        return 0;
    }
    case DISPLAY_TEXT: {
        auto const* msg = reinterpret_cast<char*>(l_param);
        
        display_text_ += " > ";
        display_text_ += msg;
        display_text_ += "\r\n";

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

        const int width = rect.right - rect.left;
        const int height = rect.top - rect.bottom;
        
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
