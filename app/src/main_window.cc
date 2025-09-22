#include "main_window.h"
#include "resource.h"
#include "win_widgets/frame_builder.h"
#include <mutex>

enum Command : uint32_t {
    kSend = 1001,
};

MainWindow::MainWindow() {
    /* Binds callbacks */
    client_.on_connect_ = [this]{ MessageBox(hwnd_, "Connected to Server", "", MB_OK); };
    client_.on_message_ = [this](const Message& msg) { MessageBox(hwnd_, msg.data, "", MB_OK);};
    server_.on_message_ = [this](Session<Message>& session, const Message& msg) { server_.SendAll(msg); };
    /* Binding of commands to lambda */
    Bind(ID_SESSION_SERVER, [this](LPARAM l_param, WPARAM w_param){
        server_enabled_ = HandleToggle(ID_SESSION_SERVER);
    });
    Bind(ID_SESSION_CLIENT, [this](LPARAM l_param, WPARAM w_param) {
        client_enabled_ = HandleToggle(ID_SESSION_CLIENT);
    });
    Bind(ID_START, [this](LPARAM l_param, WPARAM w_param) {
        Socket socket{ "127.0.0.1", 8080 };
        if (server_enabled_) {
            std::thread server_thread(&ApplicationServer<Message>::Run, &server_, socket);
            server_thread.detach();
        }
        if (client_enabled_) {
            std::thread client_thread(&ApplicationClient<Message>::Run, &client_, socket);
            client_thread.detach();
        }
    });
    Bind(kSend, [this](LPARAM l_param, WPARAM w_param) {
        std::mutex mtx;
        std::lock_guard lock_guard(mtx);

        Message buf;
        GetWindowText(message_box_->hwnd(), reinterpret_cast<char*>(&buf),sizeof(Message));
        if (strlen(buf.data) == 0) { return; }
        SetWindowText(message_box_->hwnd(), "");

        client_.Send(buf);
    });
}

bool MainWindow::HandleToggle(UINT menu_id) const {
    const HMENU menu = GetMenu(hwnd_);
    const UINT state = GetMenuState(menu, menu_id, MF_BYCOMMAND);
    const UINT toggle_state = state & MF_CHECKED ? MF_UNCHECKED : MF_CHECKED;
    
    CheckMenuItem(menu, menu_id, toggle_state);
    return toggle_state;
}

void MainWindow::HandleDrawItem(WPARAM w_param, LPARAM l_param) {
    const auto dis = reinterpret_cast<LPDRAWITEMSTRUCT>(l_param);
    if (dis->CtlID == kSend) {
        RECT client = dis->rcItem;
        FillRect(dis->hDC, &client, nullptr);

        SelectObject(dis->hDC, GetStockObject(NULL_PEN));
        const HBRUSH bkg = CreateSolidBrush(RGB(150, 150, 150));
        SelectObject(dis->hDC, bkg);

        RoundRect(dis->hDC, client.left, client.top, client.right, client.bottom, 25, 25);

        SetBkMode(dis->hDC, TRANSPARENT);
        DrawText(dis->hDC, "Hello", -1, &client, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        DeleteObject(bkg);
    }
}

void MainWindow::HandlePaint(WPARAM w_param, LPARAM l_param) {
    PAINTSTRUCT ps;
    RECT client;
    GetClientRect(hwnd_, &client);

    const HDC hdc = BeginPaint(hwnd_, &ps);

    FillRect(hdc, &client, nullptr);

    EndPaint(hwnd_, &ps);
}

void MainWindow::HandleCreate(WPARAM w_param, LPARAM l_param) {

    send_button_ = FrameBuilder<Button>()
        .Style(WS_VISIBLE | WS_CHILD)
        .Parent(hwnd_)
        .Position(50,50)
        .Size(50,50)
        .Menu(reinterpret_cast<HMENU>(kSend))
        .Build();

    message_box_ = FrameBuilder<Edit>()
        .Style(WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL)
        .Size(80, 50)
        .Position(50,0)
        .Parent(hwnd_)
        .Build();

    chat_view_ = FrameBuilder<ChatView>()
        .Style(WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL)
        .Size(600, 500)
        .Position(300 , 10)
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

