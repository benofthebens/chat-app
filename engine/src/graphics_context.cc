#include "engine/renderer/graphics_context.h"

void Engine::GraphicsContext::FillClientRect(HWND hwnd, Colour& colour) {
	RECT client;
	HBRUSH brush = CreateSolidBrush(colour.ToColourRef());
	GetClientRect(hwnd, &client);
	FillRect(hdc_, &client, brush);

	DeleteObject(brush);
}

void Engine::GraphicsContext::DrawSolidLine(Point& p, Point& p2,Colour& colour, int width) {

	HPEN pen = CreatePen(PS_SOLID, width, colour.ToColourRef());
	HPEN old_pen = static_cast<HPEN>(SelectObject(hdc_, pen));

	MoveToEx(hdc_, p.x, p.y, nullptr);
	LineTo(hdc_, p2.x, p2.y);

	SelectObject(hdc_, old_pen);
	DeleteObject(pen);
}

void Engine::GraphicsContext::OutText(const std::string& text, const Point& p) {
	TextOut(hdc_, p.x, p.y, text.data(), static_cast<int>(text.length()));
}
