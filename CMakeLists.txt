cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

project ("chat-app")

add_subdirectory("app")
add_subdirectory("network_core")
add_subdirectory("win_widgets")
enable_testing()
add_subdirectory("test")
