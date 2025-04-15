add_executable(Server
    "main.cpp"
    "../connection_base.cpp"
    "../../include/connection_base.h"
    "server.cpp"
    "../../include/server.h"
)
target_include_directories(Server PRIVATE "../../include")
