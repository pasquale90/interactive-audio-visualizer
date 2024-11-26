cmake_minimum_required(VERSION 3.10)

project(demo)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pedantic -Wall -Wextra -Wno-error")

set(SOURCE_FILES
    # src/audio.cpp
    # src/audiolizer.cpp
    # src/camera.cpp
    # src/config.cpp
    # src/console.cpp
    # src/fft.cpp
    src/main.cpp
    # src/raw.cpp
    # src/signal.cpp
    # src/videotracker.cpp
    # src/visualizer.cpp
)

# find_path(OPENCV_INCLUDE_DIR
#     NAMES opencv2/core.hpp
#     PATH_SUFFIXES opencv4
# )

# set(BTRACK_INCLUDE_DIR libraries/BTrack/src)
# set(INCLUDE_DIRECTORIES
#     include
#     ${OPENCV_INCLUDE_DIR}
#     ${BTRACK_INCLUDE_DIR}
# )

# include(CMakePrintHelpers)
# cmake_print_variables(INCLUDE_DIRECTORIES)

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

# target_link_libraries(demo ${LIBRARIES})