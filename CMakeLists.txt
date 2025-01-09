cmake_minimum_required(VERSION 3.20)
project(interactive-audio-visualizer)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pedantic -Wall -Wextra -Werror -Wunused")

# enable clang-tidy and cppcheck static analysis
# set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks='bugprone-*,performance-*,clang-analyzer-*')
# set(CMAKE_CXX_CPPCHECK cppcheck --enable=performance,warning,style)

option(BUILD_TESTS "Build the tests" OFF)

set(SOURCE_FILES
    src/gui/audiohw.cpp
    src/gui.cpp
    src/gui/camerahw.cpp
    src/gui/screenhw.cpp
    src/gui/opencvfps.cpp
    src/settings.cpp
    src/main.cpp
    src/config.cpp
    src/iav.cpp
    src/audioserver.cpp
    src/audio.cpp
    src/sine.cpp
    src/camera.cpp
    src/videotracker.cpp
    src/timer.cpp
    src/audiolizer.cpp
    # src/fft.cpp
    # src/raw.cpp
    src/visualizer.cpp
    src/trigger.cpp
)

find_path(OPENCV_INCLUDE_DIR
    NAMES opencv2/core.hpp
    PATH_SUFFIXES opencv4
)

# set(BTRACK_INCLUDE_DIR libraries/BTrack/src)
set(INCLUDE_DIRECTORIES
    include
    # include/gui    
    ${OPENCV_INCLUDE_DIR}
    # ${BTRACK_INCLUDE_DIR}
)

# include(CMakePrintHelpers)
# cmake_print_variables(INCLUDE_DIRECTORIES)

find_package(Qt6 COMPONENTS Widgets Core Gui REQUIRED)

set(LIBRARIES
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
    asound
    v4l2
    X11
    Xrandr
    sqlite3
    jack
    jackserver
    jacknet
    # fftw3
    # samplerate
    opencv_core
    opencv_highgui
    opencv_imgproc
    opencv_videoio
    opencv_video
    opencv_imgcodecs
    opencv_features2d
    opencv_tracking
    pthread
    ncurses
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRECTORIES})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

if(BUILD_TESTS)
    add_subdirectory(test)
endif()