#ifndef ENGINE_RENDERER_GRAPHICS_CONTEXT_H
#define ENGINE_RENDERER_GRAPHICS_CONTEXT_H

#include <Windows.h>
#include <string>
#include "colour.h"

namespace Engine {
	struct Point {
		int x;
		int y;
	};

	struct Rect {
		Point tr;
	    Point tl;
	    Point br;
	    Point bl;
	};

	class GraphicsContext {
	private:
		HDC hdc_;
	public:
		GraphicsContext(HDC hdc) 
			: hdc_(hdc) {}
		~GraphicsContext() = default;
		void FillClientRect(HWND hwnd, Colour& colour);
		void DrawSolidLine(Point& p, Point& p2, Colour& colour, int width);
		void OutText(const std::string& text, const Point& p);
		void DrawRct(Rect& rect);
	};
}

#endif