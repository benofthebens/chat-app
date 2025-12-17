#ifndef ENGINE_WINDOW_EVENT_H
#define ENGINE_WINDOW_EVENT_H

#include "event.h"
#include "engine/renderer/graphics_context.h"

namespace Engine {
	class WindowClosedEvent : public Event {
	public:
		WindowClosedEvent() {}

		EVENT_CLASS_TYPE(kWindowClose)
	};
	class WindowPaintEvent : public Event {
	private:
		GraphicsContext ctx_;
	public:
		WindowPaintEvent(const GraphicsContext& ctx)
		    : ctx_(ctx) {}
		GraphicsContext& GetContext() { return ctx_; }

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