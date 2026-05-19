cmake_minimum_required(VERSION 3.14)
project(GildedRose)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Library
add_library(gilded_rose src/GildedRose.cpp)
target_include_directories(gilded_rose PUBLIC include)

# Tests
enable_testing()
add_executable(gilded_rose_test
    test/GildedRoseTest.cpp
    test/GoldenMasterTest.cpp
)
target_link_libraries(gilded_rose_test gilded_rose GTest::gtest_main)
target_compile_definitions(gilded_rose_test PRIVATE
    GOLDEN_MASTER_APPROVED_FILE="${CMAKE_CURRENT_SOURCE_DIR}/test/golden-master/texttest-fixture.approved.txt"
    GOLDEN_MASTER_RECEIVED_FILE="${CMAKE_CURRENT_BINARY_DIR}/texttest-fixture.received.txt"
)

include(GoogleTest)
gtest_discover_tests(gilded_rose_test)
