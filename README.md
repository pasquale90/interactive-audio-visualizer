# Interactive Audio Visualizer
This project is intented to be demonstrated in the [Researcher's Night Greece 2023](https://researchersnight.gr/)

## Design 
- [link](https://www.canva.com/design/DAFpRsyv-38/Y-kW1xyrYH6EtCSbobD1wQ/view?utm_content=DAFpRsyv-38&utm_campaign=designshare&utm_medium=link&utm_source=publishsharelink)

## Modules
- Audio Routing (done)
- Audio Visualizer
- Visual interaction


## Rough_Time_pipeline_calc
#### audio:
- #ChunksPerSecond => 44100 samples/256 buffer_size = ~173 chunks / second
- responceTime => 1000 miliseconds / 173 chunks/second = 5.08 milliseconds 

#### camera:
- (30 fps) responceTime => 1000 miliseconds / 30 fps = ~33.3 miliseconds
- (60 fps) responceTime => 1000 miliseconds / 60 fps = ~16.7 miliseconds

#### image_analysis
- YOLO NAS => ~3-10ms https://learnopencv.com/yolo-nas/

#### display
- the minimum goal is to preserve a lower to 25 ms responce time for every frame generated in the visualization
> Thread 1
-- audio capture time for chunk = 5.08ms 
-- audio analysis time for chunk = ??
-- fft visualization == ??

> Concurrent Thread 2
-- frame capture time = 16 ms
-- frame analysis time = ~ 7 ms
-- user input proccesing = ??
-- user interaction visualization = ??





