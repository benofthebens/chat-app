cmake_minimum_required (VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

project ("chat-app")
enable_testing()
add_subdirectory("lib/googletest")

add_subdirectory("app")
add_subdirectory("network")
add_subdirectory("engine")
enable_testing()
