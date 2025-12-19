#ifndef ENGINE_PROP_BUILDER_H
#define ENGINE_PROP_BUILDER_H

#include "window_props.h"

namespace Engine {
	template <typename TProp>
	class PropsBuilder {
	private:
		TProp props_;
	public:
		PropsBuilder& Parent(HWND parent) {
			props_.parent = parent;
			return *this;
		}
		PropsBuilder& Label(const std::string& label) {
			props_.label = label;
			return *this;
		}
		PropsBuilder& Position(int x, int y) {
			props_.x = x;
			props_.y = y;
			return *this;
		}
		PropsBuilder& Size(int width, int height) {
			props_.width = width;
			props_.height = height;
			return *this;
		}
		PropsBuilder& Style(DWORD style) {
			props_.style |= style;
			return *this;
		}
		TProp Build() {
			return props_;
		}
	};

}

#endif

