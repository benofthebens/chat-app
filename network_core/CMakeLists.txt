add_library(NetworkCore "src/connection.cc" "include/network_core/connection.h"  "include/network_core/protocol_handler.h"  "src/socket.cc" "src/protocol_handler.cc" "include/network_core/session.h" "include/network_core/application_server.h" "src/session.cc" "src/application_server.cc" "include/network_core/application_client.h" "src/application_client.cc")

target_link_libraries(NetworkCore PRIVATE ws2_32)
target_include_directories(NetworkCore PUBLIC "include")
