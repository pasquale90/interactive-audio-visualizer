FROM debian:12

WORKDIR /home/iav/interactive-audio-visualizer

RUN apt-get update -y \ 
    && apt-get install -y \
    wget \
    nano \ 
    build-essential \
    cmake \
    git \
    libsamplerate0 \
    libsamplerate-dev \
    python3 \
    pkg-config \
    libasound2-dev \ 
    libx11-dev \ 
    libxrandr-dev \
    qt6-base-dev \
    libsqlite3-dev \
    libfftw3-dev \ 
    libv4l-dev

# install jack audio
RUN ln -s /usr/bin/python3 /usr/bin/python
RUN git clone https://github.com/jackaudio/jack2.git && cd jack2 && ./waf configure --prefix /usr && ./waf && ./waf install 
# add real-time leave for latter
# https://jackaudio.org/faq/linux_rt_config.html
RUN touch  /etc/security/limits.d/audio.conf \
    && echo -e '@audio   -  rtprio     95 \n@audio   -  memlock    unlimited' > /etc/security/limits.d/audio.conf
# RUN usermod -a -G audio root

#install opencv
RUN apt-get install -y libgtk2.0-dev unzip
RUN wget https://codeload.github.com/opencv/opencv/zip/refs/tags/4.10.0 \
    && wget https://codeload.github.com/opencv/opencv_contrib/zip/refs/tags/4.10.0 \
    && unzip 4.10.0 \
    && unzip 4.10.0.1 \
    && sed -i '7i #include <thread>' opencv-4.10.0/modules/gapi/test/gapi_async_test.cpp \
    && cd opencv-4.10.0 \
    && mkdir build \
    && cd build \
    && cmake -D CMAKE_BUILD_TYPE=RELEASE -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-4.10.0/modules -D WITH_QT=OFF -D WITH_GTK=ON -D WITH_GSTREAMER=OFF WITH_OPENMP=ON -D CMAKE_CXX_STANDARD=17 -D CMAKE_CXX_FLAGS="-std=c++17" ../../opencv-4.10.0 \
    && cmake --build . \
    && make install 
RUN rm -r ./jack2 ./opencv* ./4.10.0*

# install iav
RUN mkdir ./src ./include ./data
COPY src ./src
COPY include ./include
COPY CMakeLists.txt .

RUN mkdir build && cd build \
    && cmake ../ \
    && cmake --build . \
    && cp interactive-audio-visualizer ../

ENTRYPOINT ["./interactive-audio-visualizer"]