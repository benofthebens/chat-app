#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "client.h"
#include "server.h"
#include <thread>
#include "window_base.h"

#define DISPLAY_TEXT 4
#define START_SERVER 5
#define START_CLIENT 6
#define SEND_MESSAGE 7

class MainWindow : public WindowBase<MainWindow> {
private:
    HWND text_window_ = nullptr;
    HWND display_window_ = nullptr;
    Server* server_ = nullptr;
    Client* client_ = nullptr;
    std::string display_text_ = " ";
    int handle_command(WPARAM w_param, LPARAM l_param);
public:
    LPCSTR class_name() const override;
    LRESULT handle_message(UINT msg, WPARAM w_param, LPARAM l_param) override;
};

#endif