#ifndef ENGINE_WIDGETS_TEXT_INPUT_H
#define ENGINE_WIDGETS_TEXT_INPUT_H

#include "engine/panel.h"

namespace Engine {
	struct TextInputProps : PanelProps {
		TextInputProps() {
			class_name = "EDIT";
		}
	};
	class TextInput : public Panel {
	public:
		TextInput(const TextInputProps& props) 
			: Panel(props) { }
		std::string GetText() {
			int len = GetWindowTextLength(GetHwnd());
			std::string text(len + 1, '\0');
			GetWindowText(GetHwnd(), text.data(), len + 1);
			text.resize(len);
			return text;
		}
	};
}

#endif