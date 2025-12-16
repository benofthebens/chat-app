cmake_minimum_required (VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

project ("chat-app")
enable_testing()
add_subdirectory("lib/googletest")

add_subdirectory("app")
add_subdirectory("network_core")
<<<<<<< HEAD
add_subdirectory("engine")
enable_testing()
add_subdirectory("test")
=======
add_subdirectory("win_widgets")
>>>>>>> 728ecc97f982a7006ebb06c8e7a4da73e3a4b4bc
