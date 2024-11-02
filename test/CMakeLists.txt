cmake_minimum_required(VERSION 3.25)
project(Gtest1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)


enable_testing()

set(INCLUDE_DIRECTORIES
    ../include
)

set(SOURCE_FILES
    allTests.cpp
    barTest.cpp
    fooTest.cpp
    ../src/foo.cpp
    ../src/bar.cpp
)
add_executable(test_all ${SOURCE_FILES})

target_include_directories(test_all PRIVATE ${INCLUDE_DIRECTORIES})

target_link_libraries(
    test_all
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_all)
