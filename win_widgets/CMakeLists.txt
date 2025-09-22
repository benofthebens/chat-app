project("win-widgets")

set(SRC_FILES 
	"src/window.cc"
	"src/button.cc"
)
set(INCLUDE_FILES 
	"include/win_widgets/frame.h"
	"include/win_widgets/button.h"
	"include/win_widgets/frame_builder.h"
	"include/win_widgets/window.h"
	"include/win_widgets/edit.h"
)

add_library(WinWidgets SHARED ${SRC_FILES} ${INCLUDE_FILES})
add_subdirectory("test")  

target_include_directories(WinWidgets PUBLIC "include/")