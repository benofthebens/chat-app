cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

project ("chat-app")
enable_testing()
add_subdirectory("lib/googletest")

add_subdirectory("app")
add_subdirectory("network_core")
add_subdirectory("win_widgets")
