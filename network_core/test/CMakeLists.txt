
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_executable(NetworkCoreTests "test.cc")

target_link_libraries(NetworkCoreTests PRIVATE NetworkCore gtest_main)
include(GoogleTest)
gtest_discover_tests(NetworkCoreTests)