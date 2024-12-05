cmake_minimum_required(VERSION 3.20)
project(IAVtests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(INCLUDE_DIRECTORIES
    ../include
)

set(SOURCE_FILES
    allTests.cpp
    ../src/settings.cpp
    ../src/config.cpp
)
add_executable(test_all ${SOURCE_FILES})

target_include_directories(test_all PRIVATE ${INCLUDE_DIRECTORIES})

target_link_libraries(
    test_all
    GTest::gtest_main
    sqlite3
)
