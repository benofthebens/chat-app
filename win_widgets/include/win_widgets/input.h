#ifndef INPUT_H
#define INPUT_H

#include "frame.h"

#define EDIT_WC "EDIT"

class Input final : public Frame<Input> {
public:
    LPCSTR class_name() override { return EDIT_WC; }
    Input() = default;
    ~Input() override = default;
};

#endif
