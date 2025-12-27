#include "engine/renderer/font.h"
using namespace Engine;

FontBuilder& FontBuilder::Height(INT32 height) {
    props_.height_ = height;
    return *this;
}

FontBuilder& FontBuilder::Width(INT32 width) {
    props_.width_ = width;
    return *this;
}

FontBuilder& FontBuilder::Escapement(INT32 escapement) {
    props_.escapement_angle_ = escapement;
    return *this;
}

FontBuilder& FontBuilder::Weight(INT32 weight) {
    props_.weight_ = weight;
    return *this;
}

FontBuilder& FontBuilder::Italic() {
    props_.italic_ = TRUE;
    return *this;
}

FontBuilder& FontBuilder::Underline() {
    props_.underline_ = TRUE;
    return *this;
}

FontBuilder& FontBuilder::StrikeOut() {
    props_.strike_out_ = TRUE;
    return *this;
}

FontBuilder& FontBuilder::Family(DWORD family) {
    props_.family_ = family;
    return *this;
}

FontBuilder& FontBuilder::Name(LPCSTR name) {
    props_.name_ = name;
    return *this;
}

FontProps FontBuilder::Build() {
    return props_;
}

Font::Font(const FontProps& props) {
    hfont_ = CreateFont(
        props.height_,
	    props.width_,
	    props.escapement_angle_, 
	    props.orientation_,
        props.weight_,
        props.italic_,
        props.underline_,
        props.strike_out_,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, 
        CLEARTYPE_QUALITY, 
        DEFAULT_PITCH | props.family_,
        props.name_
    );
}

Font::~Font() {
    if (hfont_) {
        DeleteObject(hfont_);
    }
}
