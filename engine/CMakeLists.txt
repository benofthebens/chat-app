project("win-widgets")

set(CMAKE_CXX_STANDARD 17)

add_library(Engine "src/window.cc" "include/engine/window.h")

target_include_directories(Engine PUBLIC "include/")