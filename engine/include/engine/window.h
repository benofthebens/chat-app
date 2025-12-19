#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include <functional>
#include <memory>
#include <Windows.h>

#include "window_props.h"
#include "events/event.h"
#include "events/window_events.h"
#include "renderer/graphics_context.h"

namespace Engine {
    class Panel;
    using EventCallback = std::function<void(Event&)>;

    static constexpr auto kEngineWindow = "ENGINE_WINDOW";

    class Window {
    private:
        HWND hwnd_ = nullptr;
        HINSTANCE hinstance_ = nullptr;
        uint32_t width_;
        uint32_t height_;
        const char* label_;
        EventCallback callback_ = nullptr;
    public:
        explicit Window(const WindowProps& props) { OnInit(props); }
        ~Window() = default;

        static std::shared_ptr<Window> Create(const WindowProps& props);
        static void PollEvents();
        void RaiseEvent(Event& event) const;

        void SetEventCallback(const EventCallback& callback) { callback_ = callback; }

        HWND GetHwnd() const { return hwnd_; }
        uint32_t GetWidth() const { return width_; }
        uint32_t GetHeight() const { return height_; }
        const std::string& GetLabel() const { return label_; }
    private:
        void OnInit(const WindowProps& props);
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
        LRESULT HandleMessage(UINT msg, WPARAM w_param, LPARAM l_param);
    };
}    

#endif