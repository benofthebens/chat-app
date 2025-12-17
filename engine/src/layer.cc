#include "engine/layers/layer.h"
#include "engine/application.h"

using namespace Engine;

void Layer::QueueTransition(std::unique_ptr<Layer> to_layer) {
	auto& layerStack = Application::Get().layer_stack_;
	for (auto& layer : layerStack) {
		if (layer.get() == this) {
			layer = std::move(to_layer);
			return;
		}
	}
}
