add_executable(App WIN32 "src/main_window.cc" "src/main.cc" "src/chat_view.cc" "include/app/chat_view.h" "resource/resource.h" "include/app/chat_app.h" "include/app/main_window.h")

target_include_directories(App PRIVATE "include/app" "resource/")

target_sources(App PRIVATE "resource/menu.rc")

target_link_libraries(App PUBLIC NetworkCore WinWidgets)