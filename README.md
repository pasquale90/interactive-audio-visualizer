# Interactive Audio Visualizer

### Description
This project develops a novel interactive tool that aims to provide a real-time audiovisual experience to the user. The envisioned objective of IAV is to expose people with limited movement capabilities or with a limited musical-domain background in general, to access a positive musical stimulus in a ridiculously simplified manner (such as moving hands) so that they could be motivated to get involved in art-making.

   
### Video Demo
    
[![Interactive Audio Visualizer demo gif](iav.gif)](https://youtu.be/y8mJdEyd31Y)


 :notes: <sup>*click on frame to navigate to my video demonstration content on Youtube*</sup> :sound:


### Contents (will be updated soon)

- [Documentation](docs/readme.md)
- [preliminary analysis steps](docs/preliminary_steps.md)
- [UML desing](uml.drawio) (will be updated soon)
- build instructions (will be provided soon)

#### Libraries
- real-time audio routing : [jack audio](https://jackaudio.org/)
- image : [OpenCV](https://opencv.org/)
- beat tracking : [BeatTrack](https://github.com/adamstark/BTrack)
- fast fourier transform : [FFTW](http://fftw.org/)
- tracking algorithm : [OpenCV CSRT/KFC/BOOSTING](https://docs.opencv.org/3.4/d9/df8/group__tracking.html)
  - *for the real-time tracking feature, more than 15 c++ based methods has been tested (including hand, face, skeleton tracking and generic object detection/segmentation or optical flow), but discarded from the pipeline due to constraints met during development. An detailed report will be uploaded soon... Some of them are the following:
     - [YOLO-NAS-onnx](https://github.com/pasquale90/YOLO-NAS-onnxruntime)
     - [openpose](https://github.com/pasquale90/openpose)
     - [deepstream pose estimation](https://github.com/NVIDIA-AI-IOT/deepstream_pose_estimation)
     - [flownet2](https://github.com/lmb-freiburg/flownet2)
     - [FaceTracker](https://github.com/kylemcdonald/FaceTracker)
     - [BodySkeletonTracker](https://github.com/derzu/BodySkeletonTracker)
     - ...

### Requirements and compilation insructions
*Will be uploaded soon*

### Future steps involve the development of the following additional features: 
- [ ] enriched visualization (i.e. visualize FFT)
- [ ] replace linear audio frequency transition with a grid of predefined notes.
- [ ] add a drum transcription feature to enable the creation of rhythm patterns
- [ ] Docker support (if feasible due to hardware constraints)
- [ ] Fixes reported in the [todos](todos) file
