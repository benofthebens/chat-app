#include <chrono>
#include <Windows.h>
#include "engine/window_props.h"
#include "app/chat_app.h"
#include "app/network_layer.h"
#include "app/user.h"

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int n_cmd_show) {

    User::Get().Initialize();
    WindowProps props = { "Chat App", 1200, 600 };

    Application application(props);
    application.PushLayer<NetworkLayer>();
    application.PushLayer<ChatAppLayer>();
    application.Run();

    return 0;
}
