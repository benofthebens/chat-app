#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <memory>
#include <mutex>

#include "chat_view.h"
#include "resource.h"
#include "win_widgets/frame_builder.h"
#include "win_widgets/button.h"
#include "win_widgets/window.h"
#include "win_widgets/input.h"

#define MAIN_WINDOW_WC "WIN"

inline bool HandleToggle(HWND hwnd, UINT menu_id) {
    const HMENU menu = GetMenu(hwnd);
    const UINT state = GetMenuState(menu, menu_id, MF_BYCOMMAND);
    const UINT toggle_state = state & MF_CHECKED ? MF_UNCHECKED : MF_CHECKED;
    
    CheckMenuItem(menu, menu_id, toggle_state);

    return toggle_state;
}

class MainWindow final : public Window<MainWindow> {
private:
    bool client_enabled_ = false;
    bool server_enabled_ = false;
    Client client_ = Client("127.0.0.1", 8080);
    Server server_ = Server("127.0.0.1", 8080);
    std::unique_ptr<Button> send_button_ = nullptr;
    std::unique_ptr<Input> message_box_ = nullptr;
    std::unique_ptr<ChatView> chat_view_ = nullptr;
public:
    MainWindow();
    LPCSTR class_name() override { return MAIN_WINDOW_WC; }
private:
    void AddMessage(Message* message) {
        chat_view_->PushMessage(message);
        InvalidateRect(chat_view_->hwnd(), nullptr, TRUE);
    }
    void HandlePaint(WPARAM w_param, LPARAM l_param) override;
    void HandleCreate(WPARAM w_param, LPARAM l_param) override;
    void HandleSize(WPARAM w_param, LPARAM l_param) override;
};
#endif