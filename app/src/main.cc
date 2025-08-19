#pragma comment(lib, "Ws2_32.lib")

#include "main_window.h"

#include "chat_app.h"
#include "win_widgets/frame_builder.h"
#include <Windows.h>
#include "resource.h"

//SendMessage(send_hwnd, BM_SETIMAGE, IMAGE_ICON, reinterpret_cast<LPARAM>(send_icon));

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int n_cmd_show) {

    //send_icon = LoadIcon(h_instance, MAKEINTRESOURCE(IDB_SEND_IMG));

    const auto window = FrameBuilder<MainWindow>()
        .Name("chat-app")
        .Style(WS_OVERLAPPEDWINDOW)
        .Build();

    const HMENU menu = LoadMenu(h_instance,  MAKEINTRESOURCE(IDR_MAIN_MENU));

    SetMenu(window->hwnd(), menu);
    ShowWindow(window->hwnd(), n_cmd_show);

    CheckMenuItem(menu, ID_SESSION_SERVER, MF_UNCHECKED);
    CheckMenuItem(menu, ID_SESSION_CLIENT, MF_UNCHECKED);

    MSG msg = {};
    // Dequeues the message queue and store at &msg
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
