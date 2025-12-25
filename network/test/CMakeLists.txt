
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_executable(NetworkTest "connection_test.cc" "socket_test.cc" "session_test.cc")
target_link_libraries(NetworkTest PUBLIC Network gtest_main GTest::gmock GTest::gmock_main)
include(GoogleTest)
gtest_discover_tests(NetworkTest)