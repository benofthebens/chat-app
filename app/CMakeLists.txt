add_executable(App WIN32 "src/main.cc" "src/chat_app.cc" "include/app/chat_app.h")

target_include_directories(App PRIVATE "include/")

target_link_libraries(App PUBLIC Engine NetworkCore)