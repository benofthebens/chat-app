#ifndef ENGINE_WINDOW_PROPS_H
#define ENGINE_WINDOW_PROPS_H

#include <string>

namespace Engine {

struct WindowProps {
	std::string title;
	uint32_t width;
	uint32_t height;

	WindowProps(const std::string& title = "", uint32_t width = 1200, uint32_t height = 600)
		: title(title), width(width), height(height) {
	}
};

}

#endif