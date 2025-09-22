set(SRC_FILES 
    "src/connection.cc" 
    "src/socket.cc"
    "src/protocol_handler.cc" 
    "src/session.cc" 
    "src/application_server.cc" 
    "src/application_client.cc"
)
set(INCLUDE_FILES
    "include/network_core/connection.h"  
    "include/network_core/protocol_handler.h"  
    "include/network_core/session.h" 
    "include/network_core/application_server.h" 
    "include/network_core/application_client.h" 
    "include/network_core/socket.h" 
)
add_library(NetworkCore ${SRC_FILES} ${INCLUDE_FILES})
add_subdirectory("test")  
target_link_libraries(NetworkCore PRIVATE WS2_32)
target_include_directories(NetworkCore PUBLIC "include")
