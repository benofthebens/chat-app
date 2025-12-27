
set(CMAKE_CXX_STANDARD 17)

add_library(Engine "src/font.cc" "src/panel.cc" "src/graphics_context.cc" "src/layer.cc"  "src/application.cc" "src/window.cc" "include/engine/window.h" "include/engine/application.h" "include/engine/window_props.h" "include/engine/events/event.h" "include/engine/events/window_events.h" "include/engine/layers/layer.h" "include/engine/renderer/graphics_context.h" "include/engine/renderer/colour.h" "include/engine/panel.h" "include/engine/widgets/text_input.h" "include/engine/widgets/button.h" "include/engine/props_builder.h" "include/engine/renderer/font.h")

target_include_directories(Engine PUBLIC "include/")