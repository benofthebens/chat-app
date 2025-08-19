#ifndef INPUT_H
#define INPUT_H
#include "frame.h"
class Input : public Frame<Input> {
public:
    LPCSTR class_name() override { return "EDIT"; }
    Input() = default;
    ~Input() = default;
};

#endif
