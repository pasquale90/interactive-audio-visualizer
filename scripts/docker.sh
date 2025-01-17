# docker build
docker build -t iav:latest .
# docker run
docker run -it --rm --privileged -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix --device /dev/snd -v /dev:/dev iav:latest
