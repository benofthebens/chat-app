#ifndef ENGINE_LAYER_H
#define ENGINE_LAYER_H

#include <memory>

#include "engine/window.h"
#include "engine/events/event.h"
#include "engine/renderer/graphics_context.h"

namespace Engine {
	class Layer {
	public:
		virtual ~Layer() = default;
		virtual void OnEvent(Event& event) {}
		virtual void OnRender(GraphicsContext& ctx) {}
		virtual void OnAttach(Window& window) {}
		virtual void OnUpdate() {}

		template<typename T, typename... Args>
		void TransitionTo(Args&&... args) {
			QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
		}
	private:
		void QueueTransition(std::unique_ptr<Layer> to_layer);
	};
}

#endif 