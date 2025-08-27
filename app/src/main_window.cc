#include "main_window.h"

enum Command : uint32_t {
    kSend = 1001,
};

MainWindow::MainWindow() {
    Bind(kSend, [this](WPARAM w_param, LPARAM l_param) { client_.SendRequest(); });
    Bind(ID_SESSION_SERVER, [this](WPARAM w_param, LPARAM l_param) {
        server_enabled_ = HandleToggle(ID_SESSION_SERVER);
    });
    Bind(ID_SESSION_CLIENT, [this](WPARAM w_param, LPARAM l_param) {
        client_enabled_ = HandleToggle(ID_SESSION_CLIENT);
    });
    Bind(ID_START, [this](WPARAM w_param, LPARAM l_param) {
        if (server_enabled_) { server_.Start(); }
        if (client_enabled_) { client_.Start(); }
    });
    client_.set_message_receiver([this](Message* msg) { AddMessage(msg); });
    client_.set_message_sender([this](Message* msg) {
        std::mutex mtx;
        std::lock_guard lock_guard(mtx);
        
        GetWindowText(message_box_->hwnd(), reinterpret_cast<char*>(&msg->data), kMaxDataSize - 1);
        if (strlen(msg->data) == 0) { return; }
        SetWindowText(message_box_->hwnd(), "");
    });
}

bool MainWindow::HandleToggle(UINT menu_id) const {
    const HMENU menu = GetMenu(hwnd_);
    const UINT state = GetMenuState(menu, menu_id, MF_BYCOMMAND);
    const UINT toggle_state = state & MF_CHECKED ? MF_UNCHECKED : MF_CHECKED;
    
    CheckMenuItem(menu, menu_id, toggle_state);
    return toggle_state;
}

void MainWindow::HandlePaint(WPARAM w_param, LPARAM l_param) {
    PAINTSTRUCT ps;
    RECT client_rect;
    GetClientRect(hwnd_, &client_rect);

    const HDC hdc = BeginPaint(hwnd_, &ps);

    FillRect(hdc, &client_rect, nullptr);

    EndPaint(hwnd_, &ps);
}

void MainWindow::HandleCreate(WPARAM w_param, LPARAM l_param) {
    send_button_ = FrameBuilder<Button>()
        .Style(WS_VISIBLE | WS_CHILD)
        .Parent(hwnd_)
        .Size(50,50)
        .Menu(reinterpret_cast<HMENU>(kSend))
        .Build();

    message_box_ = FrameBuilder<Input>()
        .Style(WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL)
        .Size(80, 50)
        .Position(50,0)
        .Parent(hwnd_)
        .Build();

    chat_view_ = FrameBuilder<ChatView>()
        .Style(WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL)
        .Size(600, 500)
        .Position((ClientWidth() / 2) - 300 , 10)
        .Parent(hwnd_)
        .Build();
}

void MainWindow::HandleSize(WPARAM w_param, LPARAM l_param) {
    const WORD cx = LOWORD(l_param) / 2;
    const WORD cy = HIWORD(l_param) / 2;

    const int chat_view_x = 7 + cx / 2;

    SetWindowPos(chat_view_->hwnd(), nullptr, chat_view_x, 7, cx - 2 * 7, cy - 2 * 7, SWP_NOZORDER);
    SetWindowPos(message_box_->hwnd(), nullptr, chat_view_x, cy, cx - 2 * 7, 50, SWP_NOZORDER);
    SetWindowPos(send_button_->hwnd(),nullptr,chat_view_x + cx - 2 * 7,cy,50,50,SWP_NOZORDER);
}

