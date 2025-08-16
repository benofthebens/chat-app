#ifndef CHAT_VIEW_H 
#define CHAT_VIEW_H 

#include "chat_app.h"
#include <string>
#include <Windows.h>
#include "message.h"

#define CHAT_VIEW_WC "CHAT_VIEW"

void AddMessage(HWND hwnd, Message* msg);
void ChatViewRegister();
inline void ChatViewUnregister() {}

#endif