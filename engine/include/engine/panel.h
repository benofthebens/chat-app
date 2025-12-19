#ifndef ENGINE_PANEL_H
#define ENGINE_PANEL_H

#include <cstdint>
#include <map>
#include <Windows.h>

#include "application.h"
#include "window.h"

namespace Engine {
    class Panel {
    private:
        HWND parent_ = nullptr;
        HWND hwnd_ = nullptr;
        std::vector<std::unique_ptr<Panel>> children_;
        uint32_t id_;
        const char* label_;
        uint32_t width_;
        uint32_t height_;
        int32_t x_;
        int32_t y_;
        static uint32_t cid_;
    public:
        Panel(const PanelProps& props) { OnInit(props); }
        virtual ~Panel() = default;

        HWND GetHwnd() const { return hwnd_; }
        HWND GetParent() const { return parent_; }
        uint32_t GetWidth() const { return width_; }
        uint32_t GetHeight() const { return height_; }
        uint32_t GetId() const { return id_; }

        void Show() const { ShowWindow(hwnd_, SW_SHOW); }
        void Hide() const { ShowWindow(hwnd_, SW_HIDE); }

        template<typename TPanel, typename TProps>
        TPanel* AddChild(TProps& props) {
            props.parent = hwnd_;
            auto panel = std::make_unique<TPanel>(props);
            auto* ptr = panel.get();
            children_.push_back(std::move(panel));
            return ptr;
        }

        virtual void OnPaint(GraphicsContext& ctx) {}
        virtual void OnResize(uint32_t width, uint32_t height) {}
        virtual void OnMouseDown(int x, int y) {}
        virtual void OnMouseUp(int x, int y) {}
        virtual void OnMouseMove(int x, int y) {}
        virtual void OnKeyDown(int key_code) {}
        virtual void OnKeyUp(int key_code) {}
        virtual void OnScroll(int delta) {}

    private:
        void Register(WNDCLASSEX& wc);
        void OnInit(const PanelProps& props);
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
        LRESULT HandleMessage(UINT msg, WPARAM w_param, LPARAM l_param);
    };
}
#endif 