#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <mutex>

#include "client.h"
#include "server.h"
#include "window_base.h"
#include "dialog_window.h"

class MainWindow : public WindowBase<MainWindow> {
private:
    HWND text_window_ = nullptr;
    HWND display_window_ = nullptr;
    DialogWindow dialog_window_;

    Server* server_ = nullptr;
    Client* client_ = nullptr;
    std::string display_text_;
    std::mutex text_mutex_;
    int handle_command(WPARAM w_param, LPARAM l_param);
public:
    ~MainWindow() override;
    LPCSTR class_name() const override;
    LRESULT handle_message(UINT msg, WPARAM w_param, LPARAM l_param) override;
};

#endif