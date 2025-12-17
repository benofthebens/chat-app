#include "engine/renderer/graphics_context.h"

void Engine::GraphicsContext::DrawSolidLine(Point& p, Point& p2,Colour& colour, int width) {

	HPEN pen = CreatePen(PS_SOLID, width, colour.ToColourRef());
	HPEN old_pen = static_cast<HPEN>(SelectObject(hdc_, pen));

	MoveToEx(hdc_, p.x, p.y, nullptr);
	LineTo(hdc_, p2.x, p2.y);

	SelectObject(hdc_, old_pen);
	DeleteObject(pen);
}
