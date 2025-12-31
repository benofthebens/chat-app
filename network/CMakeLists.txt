set(SRC_FILES 
    "src/application_server.cc"
    "src/application_client.cc"
    "src/connection.cc" 
    "src/session.cc"
    "src/socket.cc"
)

set(INCLUDE_FILES
    "include/network/connection.h"  
    "include/network/protocol.h"  
    "include/network/session.h" 
    "include/network/application_server.h" 
    "include/network/application_client.h" 
    "include/network/socket.h" 
)

add_library(Network ${SRC_FILES} ${INCLUDE_FILES})
add_subdirectory("test")  
target_link_libraries(Network PUBLIC WS2_32)
target_include_directories(Network PUBLIC "include")
