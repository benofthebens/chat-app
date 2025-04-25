add_executable(App WIN32
    "main.cpp"
    "main_window.cpp"
 )

target_include_directories(App PRIVATE 
    "../../include"
    "../../include/network_core"
)

target_link_libraries(App PRIVATE NetworkCore)