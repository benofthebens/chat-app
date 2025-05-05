#ifndef DIALOG_WINDOW_H
#define DIALOG_WINDOW_H

#include "window_base.h"

struct Socket {
    char* host;
    unsigned int port;
};
class DialogWindow : public WindowBase<DialogWindow> {
private:
    HWND host_input_window_ = nullptr;
    HWND port_input_window_ = nullptr;
    HWND connect_button_window_ = nullptr;
    UINT target_command_ = 0;
    int handle_command(WPARAM w_param, LPARAM l_param);
public:
    ~DialogWindow() override;
    LPCSTR class_name() const override;
    LRESULT handle_message(UINT msg, WPARAM w_param, LPARAM l_param) override;
    void set_target_command(const unsigned int target_command);
};

#endif // !
