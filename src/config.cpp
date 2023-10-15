#include "config.h"

Config::Config(int sr,int qnt, int bfs, int fs, int dresx, int dresy, int cresx, int cresy, int cfs):
                                                                                        sampleRate(sr),
                                                                                        quantization(qnt),
                                                                                        bufferSize(bfs),
                                                                                        fps(fs),
                                                                                        displayW(dresx),
                                                                                        displayH(dresy),
                                                                                        camResW(cresx),
                                                                                        camResH(cresy),
                                                                                        camfps(cfs)
                                                                                        {};


Config::Config(){
    sampleRate=44100;
    quantization=24;
    bufferSize=512;
    fps=25;
    displayW=1024;
    displayH=512;
    camResW=640;
    camResH=480;
    camfps=30;
}

Config::Config(const Config& c):
                            sampleRate(c.sampleRate),
                            quantization(c.quantization),
                            bufferSize(c.bufferSize),
                            fps(c.fps),
                            displayW(c.displayW),
                            displayH(c.displayH),
                            camResW(c.camResW),
                            camResH(c.camResH),
                            camfps(c.camfps)
                            {};

void Config::display(){
    
    
    std::cout<<"########## Interactive Audio Visualizer Config ##########\n";
    std::cout<<"sampling rate            \t:\t"<<sampleRate<<std::endl;
    std::cout<<"quantization             \t:\t"<<quantization<<std::endl;
    std::cout<<"buffer size              \t:\t"<<bufferSize<<std::endl;
    std::cout<<"frames per second        \t:\t"<<fps<<std::endl;
    std::cout<<"display Width            \t:\t"<<displayW<<std::endl;
    std::cout<<"display Height           \t:\t"<<displayH<<std::endl;
    std::cout<<"camera resolution width  \t:\t"<<camResW<<std::endl;
    std::cout<<"camera resolution height \t:\t"<<camResH<<std::endl;
    std::cout<<"camera frames per second \t:\t"<<camfps<<std::endl;
    std::cout<<"#########################################################\n\n";

}