#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include <functional>
#include <memory>
#include <Windows.h>

#include "window_props.h"
#include "events/event.h"
#include "events/window_events.h"

namespace Engine {
    using EventCallback = std::function<void(Event&)>;

    class Window {
    private:
        HWND hwnd_ = nullptr;
        HINSTANCE hinstance_ = nullptr;

        struct WindowData {
            std::string title;
            uint32_t width = 0;
            uint32_t height = 0;
            EventCallback callback = nullptr;
        };

        WindowData data_;
        static constexpr auto kClassName = "EngineWindow";
    public:
        explicit Window(const WindowProps& props) { OnInit(props); }
        ~Window() = default;

        static std::unique_ptr<Window> Create(const WindowProps& props);
        static void PollEvents();

        void RaiseEvent(Event& event);
        void SetEventCallback(const EventCallback& callback) { data_.callback = callback; }

        HWND GetHwnd() { return hwnd_; }
        uint32_t GetWidth() const { return data_.width; }
        uint32_t GetHeight() const { return data_.height; }
        const std::string& GetTitle() const { return data_.title; }

    private:
        void OnInit(const WindowProps& props);
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
        LRESULT HandleMessage(UINT msg, WPARAM w_param, LPARAM l_param);
    };
}    

#endif