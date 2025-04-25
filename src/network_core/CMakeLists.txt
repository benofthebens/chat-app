add_library(NetworkCore
    "client.cpp"
    "server.cpp"
    "connection_base.cpp"
)

target_link_libraries(NetworkCore PRIVATE ws2_32)
target_include_directories(NetworkCore PRIVATE "../../include/network_core")
