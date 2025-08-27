#ifndef BUTTON_H
#define BUTTON_H

#include "frame.h"

#define BUTTON_WC "BUTTON"

class Button final : public Frame<Button> {
public:
	LPCSTR class_name() override { return BUTTON_WC; }
	Button() = default;
	~Button() override = default;
};

#endif