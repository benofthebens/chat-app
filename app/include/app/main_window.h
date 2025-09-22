#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <memory>

#include "network_core/application_server.h"
#include "network_core/application_client.h"
#include "win_widgets/window.h"
#include "win_widgets/button.h"
#include "win_widgets/edit.h"
#include "chat_view.h"

#define MAIN_WINDOW_WC "MAIN_WINDOW"

constexpr int kMaxDataSize = 1024;

struct Message {
    char data[kMaxDataSize];
};

class MainWindow final : public Window<MainWindow> {
private:
    std::unique_ptr<Button> send_button_ = nullptr;
    std::unique_ptr<Edit> message_box_ = nullptr;
    std::unique_ptr<ChatView> chat_view_ = nullptr;
    ApplicationServer<Message> server_;
    ApplicationClient<Message> client_;
    bool server_enabled_ = false;
    bool client_enabled_ = false;
public:
    MainWindow();
    LPCSTR class_name() override { return MAIN_WINDOW_WC; }
private:
    bool HandleToggle(UINT menu_id) const;
    void HandleDrawItem(WPARAM w_param, LPARAM l_param) override;
    void HandlePaint(WPARAM w_param, LPARAM l_param) override;
    void HandleCreate(WPARAM w_param, LPARAM l_param) override;
    void HandleSize(WPARAM w_param, LPARAM l_param) override;
};
#endif