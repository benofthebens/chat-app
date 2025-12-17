project("win-widgets")

set(CMAKE_CXX_STANDARD 17)

add_library(Engine "src/layer.cc" "src/event.cc" "src/application.cc" "src/window.cc" "include/engine/window.h" "include/engine/application.h" "include/engine/window_props.h" "include/engine/events/event.h" "include/engine/events/window_events.h" "include/engine/layers/layer.h")

target_include_directories(Engine PUBLIC "include/")