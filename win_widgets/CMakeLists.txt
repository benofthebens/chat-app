project("win-widgets")

add_library(WinWidgets SHARED "src/window.cc" "include/win_widgets/frame.h" "include/win_widgets/button.h" "src/button.cc" "include/win_widgets/frame_builder.h"  "include/win_widgets/window.h" "include/win_widgets/input.h")

target_include_directories(WinWidgets PUBLIC "include/")