#ifndef ENGINE_EVENT_H
#define ENGINE_EVENT_H

#include <functional>
#include <string>

namespace Engine {
	enum class EventType {
		kNone = 0,
		kWindowClose,
		kWindowResize,
		kKeyPressed,
		kKeyReleased,
		kWindowPaint,
		kMessageSend,
		kMessageReceive
	};

	class Event {
	private:
		bool handled_ = false;
	public:
		virtual ~Event() {}
		bool IsHandled() const { return handled_; }
		void SetHandled(bool handled) { handled_ = handled; }
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
	class EventDispatcher {
		template <typename E>
		using EventFunc = std::function<bool(E&)>;
	private:
		Event& event_;
	public:
		EventDispatcher(Event& event)
		    : event_(event) {}
		~EventDispatcher() = default;
		
		template <typename E>
		bool Dispatch(EventFunc<E> event_callback) {
			if (event_.GetEventType() == E::GetStaticType() && !event_.IsHandled()) {
				event_.SetHandled(event_callback(*(E*)&event_));
				return true;
			}
			return false;
		}
	};

} // namespace Engine
#endif 