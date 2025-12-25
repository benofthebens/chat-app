set(SRC_FILES 
    "src/connection.cc" 
    "src/socket.cc"
     
     
     
    
)
set(INCLUDE_FILES
    "include/network/connection.h"  
    "include/network/protocol_handler.h"  
    "include/network/session.h" 
    "include/network/application_server.h" 
    "include/network/application_client.h" 
    "include/network/socket.h" 
)
add_library(Network ${SRC_FILES} ${INCLUDE_FILES})
add_subdirectory("test")  
target_link_libraries(Network PRIVATE WS2_32)
target_include_directories(Network PUBLIC "include")
