add_definitions(-DWIN32_LEAN_AND_MEAN)

add_executable(App WIN32 "src/chat_panel.cc" "src/network_layer.cc" "src/main.cc" "src/chat_app.cc" "include/app/chat_app.h" "include/app/network_layer.h" "include/app/network_events.h" "include/app/chat_panel.h"   "include/app/user.h")
target_include_directories(App PRIVATE "include/")
target_link_libraries(App PUBLIC Engine Network)

add_executable(Server "src/server.cc")
target_include_directories(Server PRIVATE "include/")
target_link_libraries(Server PUBLIC Network Engine)