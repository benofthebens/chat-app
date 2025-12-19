#ifndef ENGINE_WIDGET_BUTTON_H
#define ENGINE_WIDGET_BUTTON_H

#include "engine/panel.h"

namespace Engine {
	struct ButtonProps : PanelProps {
		std::function<void()> on_click = nullptr;
		ButtonProps() {
			class_name = "BUTTON";
			style |= BS_PUSHBUTTON | WS_BORDER;
		}
	};
	class Button : public Panel {
	public:
		Button(const ButtonProps& props) 
			: Panel(props) {
			on_click_ = props.on_click;
		}
		~Button() override = default;
		std::function<void()> on_click_;
	};
}
#endif
