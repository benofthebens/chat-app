# CMakeList.txt : CMake project for chat-app, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("chat-app")

add_subdirectory("./src")
enable_testing()
add_subdirectory("test")
