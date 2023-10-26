#include "config.h"


Config::Config(int argc, char *argv[]){
    /* arguments are provided with the following name
    * sample_rate
    * buffer_size
    * bit_quantization
    * display_frames_per_second
    * display_width
    * display_height
    * camera_resolution_width
    * camera_resolution_height
    * camera_frames_per_second
    */

    InputParser input(argc, argv); // get optional arguments
    DefaultSettings defaultConfig; // get predefined defaults

    
    // Initialize sampleRate
    if (!input.getCmdOption("--sample_rate").empty()){
        sampleRate=std::stoi(input.getCmdOption("--sample_rate"));
    }else{
        sampleRate=defaultConfig.sampleRate;
    }

    // Initialize quantization
    if (!input.getCmdOption("--bit_quantization").empty()){
        quantization=std::stoi(input.getCmdOption("--bit_quantization"));
    }else{
        quantization=defaultConfig.quantization;
    }

    // Initialize bufferSize
    if (!input.getCmdOption("--buffer_size").empty()){
        bufferSize=std::stoi(input.getCmdOption("--buffer_size"));
    }else{
        bufferSize=defaultConfig.bufferSize;
    }


    // Initialize fps
    if (!input.getCmdOption("--display_frames_per_second").empty()){
        fps=std::stoi(input.getCmdOption("--display_frames_per_second"));
    }else{
        fps=defaultConfig.fps;
    }

    // Initialize displayW
    if (!input.getCmdOption("--display_width").empty()){
        displayW=std::stoi(input.getCmdOption("--display_width"));
    }else{
        displayW=defaultConfig.displayW;
    }


    // Initialize displayH
    if (!input.getCmdOption("--display_height").empty()){
        displayH=std::stoi(input.getCmdOption("--display_height"));
    }else{
        displayH=defaultConfig.displayH;
    }


    // Initialize camResW
    if (!input.getCmdOption("--camera_resolution_width").empty()){
        camResW=std::stoi(input.getCmdOption("--camera_resolution_width"));
    }else{
        camResW=defaultConfig.camResW;
    }

    // Initialize camResH
    if (!input.getCmdOption("--camera_resolution_height").empty()){
        camResH=std::stoi(input.getCmdOption("--camera_resolution_height"));
    }else{
        camResH=defaultConfig.camResH;
    }

    
    // Initialize camfps
    if (!input.getCmdOption("--camera_frames_per_second").empty()){
        camfps=std::stoi(input.getCmdOption("--camera_frames_per_second"));
    }else{
        camfps=defaultConfig.camfps;
    }

    // Initialize radius -->  SIZE OF THE ROI BOX
    if (!input.getCmdOption("--roi_radius").empty()){
        radius=std::stoi(input.getCmdOption("--roi_radius")); // it gets the number of square pixels directly. If proportional to cameraHeight, provide the result (i.e. cameraHeight*ratio ) directly as an argument.
    }else{
        radius=defaultConfig.radius;
    }

    // Initialize roiSec -->  time to wait for capturing pattern of interest within ROI in seconds
    if (!input.getCmdOption("--roi_waiting_seconds").empty()){
        roiSec=std::stoi(input.getCmdOption("--roi_waiting_seconds"));
    }else{
        roiSec=defaultConfig.roiSec;
    }

    if (!input.getCmdOption("--tracking_algorithm").empty()){
        trackingAlg=std::stoi(input.getCmdOption("--tracking_algorithm"));
        if (trackingAlg<0 || trackingAlg>3){
            std::cerr<<"Config::Invalid user input. Tracking algorithm invalid input. Provide one of the following options in bash -- >0:CSRT ,\n 1:KFC, \n2:BOOSTING\n"<<std::endl;}
    }else{
        trackingAlg=defaultConfig.trackingAlg;
    }
    const std::string trackerTypes[3]={"CSRT", "KCF", "BOOSTING"};

    // Initialize minFrequency -->  defines the minimum frequency
    if (!input.getCmdOption("--minimum_frequency").empty()){
        minFrequency=std::stoi(input.getCmdOption("--minimum_frequency"));
    }else{
        minFrequency=defaultConfig.minFrequency;
    }

    // Initialize maxFrequency -->  defines the maximum frequency
    if (!input.getCmdOption("--maximum_frequency").empty()){
        maxFrequency=std::stoi(input.getCmdOption("--maximum_frequency"));
    }else{
        maxFrequency=defaultConfig.maxFrequency;
    }
}

Config::Config(){
    DefaultSettings defaultConfig; // get predefined defaults

    sampleRate=defaultConfig.sampleRate;
    quantization=defaultConfig.quantization;
    bufferSize=defaultConfig.bufferSize;
    minFrequency=defaultConfig.minFrequency;
    maxFrequency=defaultConfig.maxFrequency;
    fps=defaultConfig.fps;
    displayW=defaultConfig.displayW;
    displayH=defaultConfig.displayH;
    camResW=defaultConfig.camResW;
    camResH=defaultConfig.camResH;
    camfps=defaultConfig.camfps;
    radius=defaultConfig.radius;
    roiSec=defaultConfig.roiSec;
    trackingAlg=defaultConfig.trackingAlg;
}

Config::Config(const Config& c):
                            sampleRate(c.sampleRate),
                            quantization(c.quantization),
                            bufferSize(c.bufferSize),
                            minFrequency(c.minFrequency),
                            maxFrequency(c.maxFrequency),
                            fps(c.fps),
                            displayW(c.displayW),
                            displayH(c.displayH),
                            camResW(c.camResW),
                            camResH(c.camResH),
                            camfps(c.camfps),
                            radius(c.radius),
                            roiSec(c.roiSec),
                            trackingAlg(c.trackingAlg)
                            {};

Config::~Config(){
    std::cout<<"Config object destructed"<<std::endl;
}

void Config::display(){
    
    std::cout<<"########## Interactive Audio Visualizer Config ##########\n";
    std::cout<<"---------------------- audio ----------------------------\n";
    std::cout<<"sampling rate            \t:\t"<<sampleRate<<std::endl;
    std::cout<<"quantization             \t:\t"<<quantization<<std::endl;
    std::cout<<"buffer size              \t:\t"<<bufferSize<<std::endl;
    std::cout<<"mininum frequency        \t:\t"<<minFrequency<<std::endl;
    std::cout<<"maxinum frequency        \t:\t"<<maxFrequency<<std::endl;
    std::cout<<"--------------------- visual ----------------------------\n";
    std::cout<<"frames per second        \t:\t"<<fps<<std::endl;
    std::cout<<"display Width            \t:\t"<<displayW<<std::endl;
    std::cout<<"display Height           \t:\t"<<displayH<<std::endl;
    std::cout<<"--------------------- camera ----------------------------\n";
    std::cout<<"camera resolution width  \t:\t"<<camResW<<std::endl;
    std::cout<<"camera resolution height \t:\t"<<camResH<<std::endl;
    std::cout<<"camera frames per second \t:\t"<<camfps<<std::endl;
    std::cout<<"radius                   \t:\t"<<radius<<std::endl;
    std::cout<<"roiSec                   \t:\t"<<roiSec<<std::endl;
    std::cout<<"tracking algorithm       \t:\t"<<trackingAlg<<std::endl;
    std::cout<<"#########################################################\n\n";

}