#pragma comment(lib, "Ws2_32.lib")

#include <Windows.h>
#include "engine/window_props.h"

#include "app/chat_app.h"


int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int n_cmd_show) {

    constexpr WORD version_requested = MAKEWORD(2, 2); // gets the version requested
    WSADATA wsa_data;
    if (WSAStartup(version_requested, &wsa_data) != 0) { return -1; }

    Engine::WindowProps props = { "Chat App", 600, 600 };

    Engine::Application application(props);
    application.PushLayer<ChatAppLayer>();
    application.Run();

    return 0;
}
