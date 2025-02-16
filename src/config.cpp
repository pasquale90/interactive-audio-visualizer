#include <iostream>
#include "config.h"
#include "settings.h"

Config::Config(){
    
    SettingsDB settingsDB;
    settings = settingsDB.loadSettings();

	if (!settings.empty()){
		// Initialize audio configuration
		audconf.audioDevice = settings["audioDevice"];
		audconf.sampleRate.store(std::stoi(settings["sampleRate"])); // --> WILL BE ADJUSTED BY JACK
		audconf.quantization = std::stoi(settings["quantizationRatio"]);
		audconf.bufferSize.store(std::stoi(settings["bufferSize"])); // --> WILL BE ADJUSTED BY JACK
		audconf.numChannels.store(std::stoi(settings["numChannels"]));
		audconf.numChannels.store(std::min(audconf.numChannels.load(),2u)); // --> make numChannels mono or stereo and discard greater values


		// Initialize camera configuration
		camconf.device = settings["cameraDevice"];
		std::string camRes = settings["cameraResolution"];
		std::string temp = camRes.substr(0, camRes.find('x')); 
		camconf.camResW.store(std::stoi(temp)); // --> WILL BE ADJUSTED BY OPENCV
		camconf.camResH.store(std::stoi(camRes.substr(temp.length() + 1)));  // --> WILL BE ADJUSTED BY OPENCV
		camconf.frameRate.store(std::stod(settings["cameraFrameRate"]));

		// Initialize screen configuration
		std::string screenRes = settings["displayResolution"];
		temp = screenRes.substr(0, screenRes.find('x'));
		dispconf.dispResW.store(std::stoi(temp));	// --> WILL BE ADJUSTED BY OPENCV
		dispconf.dispResH.store(std::stoi(screenRes.substr(temp.length() + 1)));	// --> WILL BE ADJUSTED BY OPENCV

		// Initialize iav configuration    
		dispconf.fps.store(std::stod(settings["cameraFrameRate"]));
		if (settings["frequencyRange"] == "Narrow")
		{
			iavconf.minFrequency = 300;
			iavconf.maxFrequency = 700;
		}
		else if (settings["frequencyRange"] == "Normal")
		{
			iavconf.minFrequency = 300;
			iavconf.maxFrequency = 1500;
		}
		else if (settings["frequencyRange"] == "Wide")
		{
			iavconf.minFrequency = 100;
			iavconf.maxFrequency = 3000;
		} 
		
		if (settings["roi"] == "Small")
			iavconf.roiRadius = static_cast<int>(0.05 * camconf.camResW);
		else if (settings["roi"] == "Medium")
			iavconf.roiRadius = static_cast<int>(0.1 * camconf.camResW);
		else if (settings["roi"] == "Large")
			iavconf.roiRadius = static_cast<int>(0.15 * camconf.camResW);
		iavconf.trigger = settings["trigger"];
		iavconf.trackingAlg = settings["trackingAlgorithm"];
		// iavconf.skipFramesRatio = std::stoi(settings["skipFramesRatio"]);
		// for defining skipFramesRatio, fps/skipFramesRatio should deduce the final value of frameRate and fps variables.
	}
	if (runAtomicityCheck()){
		std::cerr<<"WARNING: Atomicity is not supported on this platform for some types"<<std::endl;
	}
}

void Config::display(){

    std::cout<<"########## Interactive Audio Visualizer Config ##########\n";
    std::cout<<"------------------ audio settings -----------------------\n";
    std::cout<<"audio device             \t:\t"<<audconf.audioDevice<<"\n";
    std::cout<<"sampling rate            \t:\t"<<audconf.sampleRate.load()<<" samples/sec"<<"\n";
    std::cout<<"quantization             \t:\t"<<audconf.quantization<<" bits"<<"\n";
    std::cout<<"buffer size              \t:\t"<<audconf.bufferSize.load()<<" samples"<<"\n";
	std::cout<<"num output channels      \t:\t"<<audconf.numChannels.load()<<" "<<"\n";
	std::cout<<"----------------- display settings ----------------------\n";
    std::cout<<"frames per second        \t:\t"<<dispconf.fps.load()<<" fps"<<"\n";
    std::cout<<"display Width            \t:\t"<<dispconf.dispResW.load()<<" pixels"<<"\n";
    std::cout<<"display Height           \t:\t"<<dispconf.dispResH.load()<<" pixels"<<"\n";
    std::cout<<"------------------ camera settings ----------------------\n";
    std::cout<<"camera device            \t:\t"<<camconf.device<<"\n"; 
    std::cout<<"camera resolution width  \t:\t"<<camconf.camResW.load()<<" pixels"<<"\n";
    std::cout<<"camera resolution height \t:\t"<<camconf.camResH.load()<<" pixels"<<"\n";
    std::cout<<"camera frame rate        \t:\t"<<camconf.frameRate.load()<<" fps"<<"\n";
    std::cout<<"------------------ iav Settings -------------------------\n";
    std::cout<<"mininum frequency        \t:\t"<<iavconf.minFrequency<<" Hz"<<"\n";
    std::cout<<"maxinum frequency        \t:\t"<<iavconf.maxFrequency<<" Hz"<<"\n";
    std::cout<<"radius                   \t:\t"<<iavconf.roiRadius<<" pixels"<<"\n";
    std::cout<<"trigger method           \t:\t"<<iavconf.trigger<<"\n";
    std::cout<<"tracking algorithm       \t:\t"<<iavconf.trackingAlg<<"\n";
	// std::cout<<"skip frames ratio        \t:\t"<<iavconf.skipFramesRatio<<"\n";
	// std::cout<<"number of skip frames	 \t:\t"<<iavconf.skipFramesRatio-1<<" frames"<<"\n";
    std::cout<<"#########################################################\n\n"<<std::endl;

}

bool Config::runAtomicityCheck() {
	
	bool warning = false;
	
	// Check for atomic lock freedom for each atomic member in ConfigStruct
	if (!std::atomic<int>::is_always_lock_free) {
		std::cerr << "Warning: atomic<int> is not lock-free!\n";
		warning = true;
	}

	if (!std::atomic<unsigned int>::is_always_lock_free) {
		std::cerr << "Warning: atomic<unsigned int> is not lock-free!\n";
		warning = true;
	}

	if (!std::atomic<double>::is_always_lock_free) {
		std::cerr << "Warning: atomic<double> is not lock-free!\n";
		warning = true;
	}

	return warning;
}

