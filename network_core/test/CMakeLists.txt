
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_executable(NetworkCoreTests "connection_test.cc" "socket_test.cc" "session_test.cc")

target_link_libraries(NetworkCoreTests PUBLIC NetworkCore gtest_main GTest::gmock GTest::gmock_main)
include(GoogleTest)
gtest_discover_tests(NetworkCoreTests)