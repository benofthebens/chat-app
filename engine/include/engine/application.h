#ifndef ENGINE_APPLICATION_H
#define ENGINE_APPLICATION_H

#include "window.h"
#include "events/window_events.h"
#include "layers/layer.h"

namespace Engine {

	class Application {
	private:
		bool running_ = false;
		std::shared_ptr<Window> window_ = nullptr;
		std::vector<std::unique_ptr<Layer>> layer_stack_;
		friend class Layer;
	public:
		explicit Application(const WindowProps& props);
		virtual ~Application() = default;
		void Run();

		template <typename TLayer>
		void PushLayer() { layer_stack_.push_back(std::make_unique<TLayer>()); }

		template<typename TLayer>
		TLayer* GetLayer() {
			for (const auto& layer : layer_stack_) {
				if (auto casted = dynamic_cast<TLayer*>(layer.get())) {
					return casted;
				}
			}
			return nullptr;
		}

		std::shared_ptr<Window> GetWindow() { return window_; }
		static Application& Get();
	protected:
		virtual void OnEvent(Event& event);
		virtual void OnInit() { running_ = true; }
		virtual void OnShutdown() {}
	};
	static Application* instance = nullptr;
}

#endif