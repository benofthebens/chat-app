#ifndef ENGINE_WINDOW_PROPS_H
#define ENGINE_WINDOW_PROPS_H

#include <string>

namespace Engine {

struct WindowProps {
	std::string label;
	uint32_t width = 1200;
	uint32_t height = 600;
	int32_t x = CW_USEDEFAULT;
	int32_t y = CW_USEDEFAULT;
	DWORD style = WS_OVERLAPPEDWINDOW;
	DWORD ex_style = 0;
};

struct PanelProps : WindowProps {
	HWND parent = nullptr;
	std::string class_name;
	PanelProps() {
		style = WS_VISIBLE | WS_CHILD;
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}
};

}

#endif