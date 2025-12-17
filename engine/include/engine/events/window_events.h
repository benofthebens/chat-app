#ifndef ENGINE_WINDOW_EVENT_H
#define ENGINE_WINDOW_EVENT_H

#include "event.h"

namespace Engine {
	class WindowClosedEvent : public Event {
	public:
		WindowClosedEvent() {}

		EVENT_CLASS_TYPE(kWindowClose)
	};
	class WindowPaintEvent : public Event {
	public:
		WindowPaintEvent() = default;
		EVENT_CLASS_TYPE(kWindowPaint)
	};

	class WindowResizeEvent : public Event {
	private:
		uint32_t width_; 
		uint32_t height_;
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: width_(width), height_(height) {
		}

		uint32_t GetWidth() const { return width_; }
		uint32_t GetHeight() const { return height_; }

		EVENT_CLASS_TYPE(kWindowResize)
	};
}
#endif