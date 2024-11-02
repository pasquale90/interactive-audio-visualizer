cmake_minimum_required(VERSION 3.10)

project(demo)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pedantic -Wall -Wextra -Wno-error")

set(SOURCE_FILES
    src/foo.cpp
    src/bar.cpp
    src/main.cpp
)

set(INCLUDE_DIRECTORIES
    include
)

include(CMakePrintHelpers)
cmake_print_variables(INCLUDE_DIRECTORIES)

# set(LIBRARIES
#     jack
#     fftw3
#     samplerate
#     opencv_core
#     opencv_highgui
#     opencv_imgproc
#     opencv_videoio
#     opencv_video
#     opencv_imgcodecs
#     opencv_features2d
#     opencv_tracking
#     pthread
# )

add_executable(demo ${SOURCE_FILES})

target_include_directories(demo PRIVATE ${INCLUDE_DIRECTORIES})

target_link_libraries(demo ${LIBRARIES})