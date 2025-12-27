#ifndef ENGINE_RENDERER_GRAPHICS_CONTEXT_H
#define ENGINE_RENDERER_GRAPHICS_CONTEXT_H

#include <Windows.h>
#include <string>
#include "colour.h"
#include "font.h"

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
		bool font_set_ = false;
		HFONT prev_font_ = nullptr;
	public:
		GraphicsContext(HDC hdc) 
			: hdc_(hdc) {}
		~GraphicsContext();
		void FillClientRect(HWND hwnd, Colour& colour);
		void DrawSolidLine(Point& p, Point& p2, Colour& colour, int width);
		void Text(const std::string& text, const Point& p, Colour colour = Colour::Black());
		void Text(const std::string& text, LPRECT rect, UINT format = 0, Colour colour = Colour::Black());
		void DrawRoundRect(LPRECT rect, int radius, const Colour& fill);
		void DrawRect(LPRECT rect, const Colour& fill);
		void SetFont(const Font& font);
		void ResetFont();
	};
}

#endif