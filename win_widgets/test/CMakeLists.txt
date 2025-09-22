
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_executable(wWidgetsTests "test.cc")

target_link_libraries(wWidgetsTests PRIVATE WinWidgets gtest_main)
include(GoogleTest)
gtest_discover_tests(wWidgetsTests)