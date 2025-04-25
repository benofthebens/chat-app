
add_subdirectory(lib/googletest)

add_executable(Test
    "main.cpp"
)

target_link_libraries(Test gtest)