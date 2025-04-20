#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <thread>
#include "main_window.h"
#include <Windows.h>

int WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
) {
    MainWindow win;
    win.create("window", WS_OVERLAPPEDWINDOW);

    ShowWindow(win.window_handle(), nCmdShow);
    MSG msg = {};
    // Dequeues the message queue and store at &msg
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
