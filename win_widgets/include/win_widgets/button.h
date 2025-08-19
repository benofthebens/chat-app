#ifndef BUTTON_H
#define BUTTON_H

#include "frame.h"

constexpr uint32_t kOnClick = 1001;

class Button : public Frame<Button> {
public:
    LPCSTR class_name() override { return "BUTTON"; }
	Button() = default;
	~Button() = default;
};

#endif