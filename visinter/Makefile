CC=g++
FLAGS=-g -pedantic -Wall -Wextra -Wno-error --verbose -std=c++11 # $(shell pkg-config --cflags opencv4) # $(shell pkg-config --libs opencv4) 
COMPILE=$(CC) $(FLAGS)

LOPENCV =-lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_video -lopencv_videostab	-lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_superres #`pkg-config opencv --cflags --libs`
IOPENCV=-L/usr/local/lib  -I/usr/local/include/opencv4 


bin/videocap:video.cpp /usr/local/include/opencv4/opencv2/opencv.hpp /usr/include/opencv4/opencv2/core/mat.hpp /usr/include/opencv4/opencv2/core.hpp
	$(COMPILE) $(IOPENCV) video.cpp -o bin/videocap $(LOPENCV) 

# bin/opencv.o:include/opencv2/opencv.hpp
# 	$(COMPILE) -c include/opencv2/opencv.hpp -o bin/opencv.o

clean:
	rm -r bin/videocap