add_executable(App WIN32
    "main.cpp"
    "../../include/window_base.h"
    "../../include/main_window.h"
    "main_window.cpp"
    "../server/server.cpp"
    "../client/client.cpp"
    "../connection_base.cpp"
 )

target_include_directories(App PRIVATE "../../include")
