#ifndef FRAME_BUILDER_H
#define FRAME_BUILDER_H 

#include <Windows.h>
#include <memory>

/**
 * @brief 
 * @tparam Frame 
 */
template <typename Frame>
class FrameBuilder {
private:
    LPCSTR name_ = "";
    DWORD style_ = 0;
    DWORD ex_style_ = 0;
    UINT x_ = CW_USEDEFAULT;
    UINT y_ = CW_USEDEFAULT;
    UINT width_ = CW_USEDEFAULT;
    UINT height_ = CW_USEDEFAULT;
    HWND parent_hwnd_ = nullptr;
    HMENU menu_ = nullptr;
public:
    FrameBuilder& Name(const LPCSTR name) {
        name_ = name;
        return *this;
    }
    FrameBuilder& Style(const DWORD style) {
        style_ = style;
        return *this;
    }
    FrameBuilder& ExStyle(const DWORD ex_style) {
        ex_style_ = ex_style;
        return *this;
    }
    FrameBuilder& Position(const UINT x, const UINT y) {
        x_ = x;
        y_ = y;
        return *this;
    }
    FrameBuilder& Size(const UINT width, const UINT height) {
        width_ = width;
        height_ = height;
        return *this;
    }
    FrameBuilder& Parent(const HWND parent_hwnd) {
        parent_hwnd_ = parent_hwnd;
        return *this;
    }
    FrameBuilder& Menu(const HMENU menu) {
        menu_ = menu;
        return *this;
    }
    std::unique_ptr<Frame> Build() {
        auto win = std::make_unique<Frame>();
        win->Create(name_, style_, ex_style_, x_, y_, width_, height_, parent_hwnd_, menu_);
        return win;
    }
};

#endif