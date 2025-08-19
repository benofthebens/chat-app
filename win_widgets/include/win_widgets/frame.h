#ifndef FRAME_H
#define FRAME_H

#include <functional>
#include <unordered_map>
#include <Windows.h>

using Event = std::function<void(WPARAM, LPARAM)>;

class IFrame {
public:
    virtual ~IFrame() = default;
    /**
     * @author Benjamin 
     * @brief Uses map of callback functions then executes corresponding callback to the cmd code (WM_CMD msg)
     * @param w_param Stores the command code
     * @param l_param If the cmd is created by a control l_param contains the handle of that control
     */
    virtual void HandleCommand(WPARAM w_param, LPARAM l_param) = 0;
    /**
     * @author Benjamin
     * @brief Handles the painting of the Frame (WM_PAINT msg)
     * @param w_param Unused (0)
     * @param l_param Unused (0)
     */
    virtual void HandlePaint(WPARAM w_param, LPARAM l_param) = 0;
    /**
     * @author Benjamin
     * @brief Handles the creation of the Frame (WM_CREATE msg)
     * @param w_param Unused(0)
     * @param l_param Pointer to CREATESTRUCT
     */
    virtual void HandleCreate(WPARAM w_param, LPARAM l_param) = 0;
    /**
     * @author Benjamin
     * @brief Handles the resizing of the Frame (WM_SIZE message).
     *
     * @param w_param Indicates the type of resizing action:
     *  - SIZE_RESTORED (0): Window returned to normal size
     *  - SIZE_MINIMIZED (1): Window minimized
     *  - SIZE_MAXIMIZED (2): Window maximized
     *  - SIZE_MAXSHOW (3): Unused
     *  - SIZE_MAXHIDE (4): unused
     *
     * @param l_param Contains the new client-area size:
     *  - LOWORD(l_param): New width of the client area (in pixels)
     *  - HIWORD(l_param): New height of the client area (in pixels)
     */
    virtual void HandleSize(WPARAM w_param, LPARAM l_param) = 0;
    /**  // NOLINT(clang-diagnostic-invalid-utf8)
     * @author Benjamin
     * @brief Handles vertical scrolling of the Frame (WM_VSCROLL message).
     *
     * @param w_param The scroll request details:
     *  - LOWORD(w_param): Scroll request code:
     *      - SB_LINEUP (0): Scroll one line up.
     *      - SB_LINE LEFT (0): Same as above (horizontal).
     *      - SB_LINE DOWN (1): Scroll one line down.
     *      - SB_LINE RIGHT (1): Same as above (horizontal).
     *      - SB_PAGE UP (2): Scroll one page up.
     *      - SB_PAGE DOWN (3): Scroll one page down.
     *      - SB_THUMB POSITION (4): User has released the thumb and the position is in HIWORD(w_param).
     *      - SB_THUMB TRACK (5): User is dragging the scroll box; position is in HIWORD(w_param).
     *      - SB_TOP (6): Scroll to top.
     *      - SB_BOTTOM (7): Scroll to bottom.
     *      - SB_END SCROLL (8): Scrolling has ended.
     *
     *  - HIWORD(w_param): Scroll position (only valid for SB_THUMBPOSITION and SB_THUMBTRACK).
     *
     * @param l_param Handle to the scroll bar control if the message is from a scroll bar control (e.g. a child scrollbar).
     *        If the scroll bar is part of the window’s standard scroll bars, this value is NULL.
     */
    virtual void HandleScroll(WPARAM w_param, LPARAM l_param) = 0;
};

/**
 * @brief 
 * @tparam Control 
 */
template <typename Control>
class __declspec(dllexport) Frame : public IFrame {
protected:
    HWND hwnd_ = nullptr;
    std::unordered_map<uint32_t, Event> handlers_ = {};
private:
    void HandleScroll(WPARAM w_param, LPARAM l_param) override {}
    void HandleCreate(WPARAM w_param, LPARAM l_param) override {}
    void HandlePaint(WPARAM w_param, LPARAM l_param) override {}
    void HandleSize(WPARAM w_param, LPARAM l_param) override {}
    void HandleCommand(WPARAM w_param, LPARAM l_param) override {
        const int cmd = LOWORD(w_param);
        const auto iterator = handlers_.find(cmd);
        if (iterator == handlers_.end()) { return; }
        iterator->second(w_param, l_param);
    }
public:
    Frame() = default;
    ~Frame() = default;
    HWND hwnd() const { return hwnd_; }

    /**
     * @brief Adds the cmd id and the callback to the map of events 
     * @param cmd The cmd id that is used when an event is triggered 
     * @param event The callback function command
     */
    void Bind(const uint32_t cmd, Event event) { handlers_[cmd] = std::move(event); }
    virtual LPCSTR class_name() = 0;

    /**
     * @brief Creates the window using params
     * @param name The name of the frame
     * @param dw_style The style of the Frame
     * @param dw_ex_style 
     * @param x The x position of the Frame
     * @param y The y position of the Frame
     * @param width The width of the Frame
     * @param height The height of the Frame
     * @param parent_hwnd The parent's handle
     * @param menu_handle The menu handle 
     * @return BOOL whether the Creation of the window has been successful 
     */
    virtual BOOL Create(const LPCSTR name, const DWORD dw_style, const DWORD dw_ex_style = 0,
                        const int x = CW_USEDEFAULT, const int y = CW_USEDEFAULT,
                        const int width = CW_USEDEFAULT, const int height = CW_USEDEFAULT,
                        const HWND parent_hwnd = nullptr, const HMENU menu_handle = nullptr) {
        hwnd_ = CreateWindowEx(dw_ex_style, class_name(),name,dw_style,
            x, y, width, height, parent_hwnd,menu_handle,GetModuleHandle(nullptr),this);

        return hwnd_ != nullptr;
    }

    /**
     * @brief Calculates the width of the client rect
     * @return -1 if hwnd_ has not been created
     * else returns width
     */
    INT Width() const {
        if (!hwnd_) { return -1; }
        RECT client;
        GetClientRect(hwnd_, &client);
        return client.right - client.left;
    }

    /**
     * @brief Calculates the height of the client rect
     * @return -1  if hwnd_ has not been created
     * else returns height
     */
    INT Height() const {
        if (!hwnd_) { return -1; }
        RECT client;
        GetClientRect(hwnd_, &client);
        return client.bottom - client.top;
    }

    /**
     * @brief Executes corresponding method based on message
     * @param hwnd the handle of the window
     * @param msg the message 
     * @param w_param Changes based on message
     * @param l_param Changes based on message 
     */
    void HandleMessage(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
        switch (msg) {
        case WM_PAINT:      HandlePaint(w_param, l_param);      break;
        case WM_COMMAND:    HandleCommand(w_param, l_param);    break;
        case WM_SIZE:       HandleSize(w_param, l_param);       break;
        case WM_CREATE:     HandleCreate(w_param, l_param);     break;
        case WM_VSCROLL:    HandleScroll(w_param, l_param);     break;
        case WM_DESTROY:    PostQuitMessage(0);         break;
        default: break;
        }
    }

    /**
     * @brief 
     * @param hwnd the handle of the window
     * @param msg the message
     * @param w_param changes based on message
     * @param l_param changes based on message
     * @return 
     */
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
        Control* instance = nullptr;

        if (msg == WM_NCCREATE) {
            auto* create_struct = reinterpret_cast<CREATESTRUCT*>(l_param);
            instance = static_cast<Control*>(create_struct->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));
            instance->hwnd_ = hwnd;
        }
        else {
            instance = reinterpret_cast<Control*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (instance) { instance->HandleMessage(hwnd, msg, w_param, l_param); }

        return DefWindowProc(hwnd, msg, w_param, l_param);
    }
};
#endif