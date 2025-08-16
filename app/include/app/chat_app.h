#ifndef CHAT_APP_H
#define CHAT_APP_H

#include <cstdint>
#include <Windows.h>

inline LONG Width(const RECT& rect) { return rect.right - rect.left; }
inline LONG Height(const RECT& rect) { return rect.bottom - rect.top; }

#endif