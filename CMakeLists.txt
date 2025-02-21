cmake_minimum_required(VERSION 3.20)
project(interactive-audio-visualizer)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pedantic -Wall -Wextra -Werror -Wunused")

option(STATIC_ANALYSIS "Apply static analysis" OFF)
option(BUILD_TESTS "Build the tests" OFF)

if(STATIC_ANALYSIS)
    # enable clang-tidy and cppcheck static analysis
    set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks='bugprone-*,performance-*,clang-analyzer-*')
    set(CMAKE_CXX_CPPCHECK cppcheck --enable=performance,warning,style)
endif()

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
    src/visualizer.cpp
    src/trigger.cpp
    src/waveform.cpp
    src/spectrogram.cpp
)

find_path(OPENCV_INCLUDE_DIR
    NAMES opencv2/core.hpp
    PATH_SUFFIXES opencv4
)

set(INCLUDE_DIRECTORIES
    include
    ${OPENCV_INCLUDE_DIR}
)

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
    opencv_core
    opencv_highgui
    opencv_imgproc
    opencv_videoio
    opencv_video
    opencv_imgcodecs
    opencv_features2d
    opencv_tracking
    pthread
    fftw3
    samplerate
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRECTORIES})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

if(BUILD_TESTS)
    add_subdirectory(test)
endif()