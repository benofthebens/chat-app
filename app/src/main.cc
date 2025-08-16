#pragma comment(lib, "Ws2_32.lib")

#include "client.h"
#include "server.h"
#include "chat_view.h"
#include "resource.h"
#include "chat_app.h"

#include <iostream>
#include <mutex>
#include <thread>
#include <Windows.h>

Client client("127.0.0.1", 8080);
Server server("127.0.0.1", 8080);

bool client_enabled = false;
bool server_enabled = false;

HWND chat_view_hwnd;
HWND client_hwnd;
HWND input_hwnd;
HWND send_hwnd;
HICON send_icon;
#define MARGIN 7

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool HandleToggle(HWND hwnd, UINT menu_id);

bool HandleToggle(HWND hwnd, UINT menu_id) {
    const HMENU menu = GetMenu(hwnd);
    const UINT state = GetMenuState(menu, menu_id, MF_BYCOMMAND);
    const UINT toggle_state = state & MF_CHECKED ? MF_UNCHECKED : MF_CHECKED;
    
    CheckMenuItem(menu, menu_id, toggle_state);

    return toggle_state;
}

LRESULT WinProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        RECT client_rect;
        GetClientRect(hwnd, &client_rect);

        const HDC hdc = BeginPaint(hwnd, &ps);

        FillRect(hdc, &client_rect, nullptr);

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_SIZE: {
        const WORD cx = LOWORD(l_param) / 2;
        const WORD cy = HIWORD(l_param) / 2;

        const int chat_view_x = MARGIN + cx / 2;

        SetWindowPos(chat_view_hwnd,nullptr, chat_view_x, MARGIN, cx - 2 * MARGIN, cy - 2 * MARGIN, SWP_NOZORDER);
        SetWindowPos(input_hwnd, nullptr, chat_view_x, cy, cx - 2 * MARGIN, 50, SWP_NOZORDER);
        SetWindowPos(send_hwnd,nullptr,chat_view_x + cx - 2 * MARGIN,cy,50,50,SWP_NOZORDER);

        return 0;
    }
    case WM_COMMAND: {
        const int cmd = LOWORD(w_param);
        switch (cmd) {
        case 102:
            client.SendRequest();
            return 0;
        case ID_SESSION_CLIENT: {
            client_enabled = HandleToggle(hwnd, cmd);
            return 0;
        }
        case ID_SESSION_SERVER: {
            server_enabled = HandleToggle(hwnd, cmd);
            return 0;
        }
        case ID_START: {
            if (server_enabled) { server.Start(); }
            if (client_enabled) { client.Start(); }
            return 0;
        }
        default:
            return 0;
        }
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    case WM_CREATE: {
        RECT rect;
        GetClientRect(hwnd, &rect);

        const int width = rect.right - rect.left;
        const int height = rect.bottom - rect.top;
        
        chat_view_hwnd = CreateWindowEx(0, CHAT_VIEW_WC, nullptr, WS_VISIBLE | WS_CHILD | WS_BORDER |  WS_VSCROLL, 0, 0, 100, 100, hwnd, (HMENU)100, GetModuleHandle(nullptr), nullptr);
        input_hwnd = CreateWindowEx(0,"EDIT","",WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE,7, height - 200, 100, 50,  hwnd, (HMENU)101, nullptr, nullptr);
        send_hwnd = CreateWindowEx(0, "BUTTON", "", WS_VISIBLE | WS_CHILD | BS_ICON , 7, height - 200, 50, 50, hwnd, (HMENU)102, nullptr, nullptr);

        SendMessage(send_hwnd, BM_SETIMAGE, IMAGE_ICON, reinterpret_cast<LPARAM>(send_icon));

        return 0;
    }
    default:
        return DefWindowProc(hwnd, msg, w_param, l_param);
    }
}

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int n_cmd_show) {

    send_icon = LoadIcon(h_instance, MAKEINTRESOURCE(IDB_SEND_IMG));

    client.set_message_receiver([](Message* msg) { AddMessage(chat_view_hwnd, msg); });
    client.set_message_sender([](Message* msg) {
        std::mutex mtx;
        std::lock_guard<std::mutex> lock_guard(mtx);
        
        GetWindowText(input_hwnd, reinterpret_cast<char*>(&msg->data), kMaxDataSize - 1);
        if (strlen(msg->data) == 0) {
            return;
        }
        SetWindowText(input_hwnd, "");
    });

    ChatViewRegister();

    WNDCLASS wnd = {};
    wnd.lpfnWndProc = WinProc;
    wnd.hInstance = GetModuleHandle(nullptr);
    wnd.lpszClassName = "WIN";

    RegisterClass(&wnd);

    const HWND window_hwnd = CreateWindowEx(0, "WIN", "Chat Room", WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,nullptr, nullptr,      h_instance,  nullptr);
    const HMENU menu = LoadMenu(h_instance,  MAKEINTRESOURCE(IDR_MAIN_MENU));

    SetMenu(window_hwnd, menu);
    ShowWindow(window_hwnd, n_cmd_show);

    CheckMenuItem(menu, ID_SESSION_SERVER, MF_UNCHECKED);
    CheckMenuItem(menu, ID_SESSION_CLIENT, MF_UNCHECKED);

    MSG msg = {};
    // Dequeues the message queue and store at &msg
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (server.is_running()) { server.Shutdown(); }
    if (client.is_running()) { client.Shutdown(); }

    return 0;
}
