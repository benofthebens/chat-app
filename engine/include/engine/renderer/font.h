#ifndef ENGINE_FONT_H
#define ENGINE_FONT_H

#include <Windows.h>

namespace Engine {
	struct FontProps {
        INT32 height_ = 0;
		INT32 width_ = 0;
		INT32 escapement_angle_ = 0;
		INT32 orientation_ = 0;
		INT32 weight_ = 0;
		DWORD italic_ = FALSE;
		DWORD underline_ = FALSE;
		DWORD strike_out_ = FALSE;
		DWORD family_ = FF_DONTCARE;
		LPCSTR name_ = "";
	};
	class FontBuilder {
	private:
		FontProps props_;
	public:
		FontBuilder& Height(INT32 height);
		FontBuilder& Width(INT32 width);
		FontBuilder& Escapement(INT32 escapement);
		FontBuilder& Weight(INT32 weight);
		FontBuilder& Italic();
		FontBuilder& Underline();
		FontBuilder& StrikeOut();
		FontBuilder& Family(DWORD family);
		FontBuilder& Name(LPCSTR name);
		FontProps Build();
	};
	class Font {
	private:
		HFONT hfont_ = nullptr;
	public:
		explicit Font(const FontProps& props);
		~Font();
		HFONT GetHandle() const { return hfont_; }

	};

}

#endif
