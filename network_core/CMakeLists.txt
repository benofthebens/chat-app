add_library(NetworkCore "src/client.cc" "src/server.cc" "src/connection_base.cc" "include/network_core/server.h" "include/network_core/client.h" "include/network_core/connection_base.h" "include/network_core/message.h")

target_link_libraries(NetworkCore PRIVATE ws2_32)
target_include_directories(NetworkCore PUBLIC "include/network_core")
