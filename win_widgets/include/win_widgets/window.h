#ifndef WINDOW_H
#define WINDOW_H
#include "frame.h"

/**
 * @brief 
 * @tparam Control 
 */
template <typename Control>
class __declspec(dllexport) Window : public Frame<Control> {
private:
    /**
     * @brief 
     */
    void Register() {
        WNDCLASS wnd = {};
        wnd.lpfnWndProc = Control::WndProc;
        wnd.hInstance = GetModuleHandle(nullptr);
        wnd.lpszClassName = class_name();

        RegisterClass(&wnd);
    }
public:
    virtual LPCSTR class_name() = 0;
    BOOL Create(const LPCSTR name, const DWORD dw_style, const DWORD dw_ex_style = 0,
                const int x = CW_USEDEFAULT, const int y = CW_USEDEFAULT,
                const int width = CW_USEDEFAULT, const int height = CW_USEDEFAULT,
                const HWND parent_hwnd = nullptr, const HMENU menu_handle = nullptr
    ) override {
        Register();
        return Frame<Control>::Create(name, dw_style, dw_ex_style, x, y, width, height, parent_hwnd, menu_handle);
    }

};

#endif