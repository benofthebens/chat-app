#ifndef ENGINE_LAYER_H
#define ENGINE_LAYER_H

#include <memory>

#include "engine/events/event.h"

namespace Engine {
	class Layer {
	public:
		virtual ~Layer() = default;

		virtual void OnEvent(Event& event) {}

		virtual void OnUpdate() {}
		virtual void OnRender() {}

		template<typename T, typename... Args>
		void TransitionTo(Args&&... args) {
			QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
		}
	private:
		void QueueTransition(std::unique_ptr<Layer> to_layer);
	};
}

#endif 