#ifndef EDIT_H
#define EDIT_H

#include "frame.h"

#define EDIT_WC "EDIT"

class Edit : public Frame<Edit> {
public:
    LPCSTR class_name() override { return EDIT_WC; }
    Edit() = default;
    ~Edit() override = default;
};

#endif
