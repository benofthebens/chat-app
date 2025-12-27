#include "engine/renderer/graphics_context.h"

using namespace Engine;

GraphicsContext::~GraphicsContext() {
	if (prev_font_) {
		DeleteObject(prev_font_);
	}
}

void GraphicsContext::FillClientRect(HWND hwnd, Colour& colour) {
	RECT client;
	HBRUSH brush = CreateSolidBrush(colour.ToColourRef());
	GetClientRect(hwnd, &client);
	FillRect(hdc_, &client, brush);

	DeleteObject(brush);
}

void GraphicsContext::DrawSolidLine(Point& p, Point& p2,Colour& colour, int width) {

	HPEN pen = CreatePen(PS_SOLID, width, colour.ToColourRef());
	HPEN old_pen = static_cast<HPEN>(SelectObject(hdc_, pen));

	MoveToEx(hdc_, p.x, p.y, nullptr);
	LineTo(hdc_, p2.x, p2.y);

	SelectObject(hdc_, old_pen);
	DeleteObject(pen);
}

void GraphicsContext::Text(const std::string& text, const Point& p, Colour colour) {

	SetTextColor(hdc_, colour.ToColourRef());
	SetBkMode(hdc_, TRANSPARENT);
	TextOut(hdc_, p.x, p.y, text.data(), static_cast<int>(text.length()));
}

void GraphicsContext::Text(const std::string& text, LPRECT rect, UINT format, Colour colour) {
	SetTextColor(hdc_, colour.ToColourRef());
	SetBkMode(hdc_, TRANSPARENT);
	DrawText(hdc_, text.data(), -1, rect, format);
}

void GraphicsContext::DrawRoundRect(LPRECT rect, int radius, const Colour& fill) {
	HBRUSH brush = CreateSolidBrush(fill.ToColourRef());
	HBRUSH old_brush = (HBRUSH)SelectObject(hdc_, brush);
	HPEN pen = CreatePen(PS_SOLID, 1, fill.ToColourRef());
	HPEN old_pen = (HPEN)SelectObject(hdc_, pen);

	::RoundRect(hdc_, rect->left, rect->top, rect->right, rect->bottom, radius, radius);

	SelectObject(hdc_, old_brush);
	SelectObject(hdc_, old_pen);
	DeleteObject(brush);
	DeleteObject(pen);
}

void GraphicsContext::DrawRect(LPRECT rect, const Colour& fill) {
	HBRUSH brush = CreateSolidBrush(fill.ToColourRef());
	::FillRect(hdc_, rect, brush);
	DeleteObject(brush);
}

void GraphicsContext::SetFont(const Font& font) {
	if (!font_set_) {
	    prev_font_ = static_cast<HFONT>(SelectObject(hdc_, font.GetHandle()));
		font_set_ = true;
	}
	else {
	    prev_font_ = static_cast<HFONT>(SelectObject(hdc_, font.GetHandle()));
	}
}

void GraphicsContext::ResetFont() {
	if (font_set_ && prev_font_) {
		SelectObject(hdc_, prev_font_);
		font_set_ = false;
	}
}

