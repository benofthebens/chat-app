add_executable(Client
    "main.cpp"
    "client.cpp"
    "../../include/client.h"
    "../connection_base.cpp"
    "../../include/connection_base.h"
)
target_include_directories(Client PRIVATE "../../include")