#ifndef WINDOW_BASE_H
#define WINDOW_BASE_H

#include <Windows.h>

template <class DerivedClass>
class WindowBase{
protected:
    virtual LPCSTR class_name() const = 0;
    virtual LRESULT handle_message(UINT msg, WPARAM w_param, LPARAM l_param) = 0;

    HWND window_handle_;
public:
    WindowBase() : window_handle_(nullptr) {}
    virtual ~WindowBase() = default;

    HWND window_handle() const { return window_handle_;}

    static LRESULT CALLBACK win_proc(HWND window_handle, UINT msg, WPARAM w_param, LPARAM l_param) {
        // Since this is a static function we cannot use this keyword so we have to load it and store it
        // we will use window_instance to store this
        DerivedClass* window_instance = nullptr;

        // If the window is being created
        if (msg == WM_NCCREATE) {
            // When a window is being created the l_param stores a CreateStruct pointe
            CREATESTRUCT* create_struct = reinterpret_cast<CREATESTRUCT*>(l_param);
            // Get a pointer to an instance of a derived class
            window_instance = reinterpret_cast<DerivedClass*>(create_struct->lpCreateParams);
            // Store the instance as a long pointer at GWLP_USERDATA
            SetWindowLongPtr(window_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window_instance));

            // set the current instance handle
            window_instance->window_handle_ = window_handle;
        }
        else {
            // Gets the instance of the DerivedClass from the GWLP_USERDATA
            window_instance = reinterpret_cast<DerivedClass*>(GetWindowLongPtr(window_handle, GWLP_USERDATA));
        }

        // if window_instance exists use the handle_message method
        if (window_instance) 
            return window_instance->handle_message(msg, w_param, l_param);

        return DefWindowProc(window_handle, msg, w_param, l_param);
    }
    BOOL create(
        const LPCSTR window_name, 
        const DWORD dw_style, 
        const DWORD dw_ex_style = 0, 
        const int x = CW_USEDEFAULT,const int y = CW_USEDEFAULT, 
        const int width = CW_USEDEFAULT, const int height = CW_USEDEFAULT,
        const HWND parent_window_handle = nullptr,
        const HMENU menu_handle = nullptr
    ) {
        WNDCLASS wnd = {};
        wnd.lpfnWndProc = DerivedClass::win_proc;
        wnd.hInstance = GetModuleHandle(nullptr);
        wnd.lpszClassName = class_name();

        RegisterClass(&wnd);

        window_handle_ = CreateWindowEx(
            dw_ex_style,
            class_name(),
            window_name,
            dw_style,
            x, y, width, height,
            parent_window_handle,
            menu_handle,
            GetModuleHandle(nullptr),
            this 
        );

        return window_handle_ ? TRUE : FALSE; 
    }
};

#endif