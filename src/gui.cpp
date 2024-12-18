#include "gui.h"
#include <cstddef>
#include <string>

#include "gui/opencvfps.h"
#include "gui/audiohw.h"
#include "gui/camerahw.h"
#include "gui/screenhw.h"


void GUI::initializeComponents(){
    
    deviceComboBox = new QComboBox();
    sampleRateComboBox = new QComboBox();
    cameraDeviceComboBox = new QComboBox();
    resolutionComboBox = new QComboBox();
    bufferSizeComboBox = new QComboBox();
    quantizationComboBox = new QComboBox();
    frameRateComboBox= new QComboBox();
    displayResolutionComboBox= new QComboBox();
    displayFrameRateComboBox= new QComboBox();
    frequencyRangeComboBox= new QComboBox();
    roiComboBox= new QComboBox();
    triggerComboBox= new QComboBox();
    trackingAlgorithmComboBox= new QComboBox();
    skipFramesSlider = new QSlider(Qt::Horizontal);  // Horizontal slider
    
    audioDeviceLabel = new QLabel("Device:");
    sampleRateLabel = new QLabel("Sample Rate:");
    cameraDeviceLabel = new QLabel("Device:");
    cameraResolutionLabel = new QLabel("Resolution:");
    bufferSizeLabel = new QLabel("Buffer Size");
    quantizationLabel = new QLabel("Quantization");
    numOutputChannelsLabel = new QLabel("Output channels");
    numOutputChannelsValue = new QLabel();
    cameraFrameRateLabel = new QLabel("Frame Rate");
    screenResolutionLabel= new QLabel("Resolution");
    screenFrameRateLabel= new QLabel("Frames per second");
    iavFrequencyRangeLabel= new QLabel("Frequency range");
    iavRegionOfInterestLabel= new QLabel("ROI");
    iavTriggerLabel= new QLabel("Trigger");
    iavTrackingAlgLabel= new QLabel("Tracking algorithm");
    accuracyLabel = new QLabel("Accuracy");
    cpuLoadLabel = new QLabel("Economy");

    audioDeviceLabel->setToolTip("Choose audio output device.");
    sampleRateLabel->setToolTip("Set samples per second.");
    cameraDeviceLabel->setToolTip("Select camera input device.");
    cameraResolutionLabel->setToolTip("Set camera capture resolution.");
    bufferSizeLabel->setToolTip("Configure audio buffer size.");
    quantizationLabel->setToolTip("Quantization range for digitalizing audio data");
    numOutputChannelsLabel->setToolTip("Number of output channels (>2 is Stereo)");
    cameraFrameRateLabel->setToolTip("Set camera frame rate (currently fixed).");
    screenResolutionLabel->setToolTip("Select screen resolution.");
    screenFrameRateLabel->setToolTip("Set screen frame rate (currently fixed).");
    iavFrequencyRangeLabel->setToolTip("Define audio frequency range for sound generation");
    iavRegionOfInterestLabel->setToolTip("Set region of interest size (window size for photshooting pattern).");
    iavTriggerLabel->setToolTip("Choose capturing method.");
    iavTrackingAlgLabel->setToolTip("Select object tracking algorithm.");
    accuracyLabel->setToolTip("Do not skip frames.");
    cpuLoadLabel->setToolTip("Skip frames.");
    skipFramesSlider->setToolTip("CURRENTLY UNSUPPORTED");

    // create a layout for the #Outchannels
    numChannelsLayout = new QHBoxLayout();
    numChannelsLayout->addWidget(numOutputChannelsLabel);
    numChannelsLayout->addWidget(numOutputChannelsValue);
}


void GUI::addExplanations(){

    // Add explanation to the audio devices,
    for (size_t i = 0; i<audioExplanations.size(); ++i){
        deviceComboBox->setItemData(i, QString::fromStdString( audioExplanations[i] ), Qt::ToolTipRole);
    }
    
    // .. to the frame rates of both camera and screen,
    approxFps = getCVfps_approx(cameraDeviceComboBox->currentText().toStdString().c_str());
    frameRateComboBox->setItemData(0, "≈"+ QString::number( approxFps )+" fps detected hardware capability", Qt::ToolTipRole);
    displayFrameRateComboBox->setItemData(0, "≈"+ QString::number( approxFps )+" fps detected hardware capability", Qt::ToolTipRole);

    // ... to the roi comboBox options,
    roiComboBox->setItemData(0, "5% of camera's capture resolution.", Qt::ToolTipRole);
    roiComboBox->setItemData(1, "10% of camera's capture resolution.", Qt::ToolTipRole);
    roiComboBox->setItemData(2, "15% of camera's capture resolution.", Qt::ToolTipRole);

    // .. to the capturing method,
    triggerComboBox->setItemData(0, "Capturing is initialized manually, using the Space Bar key", Qt::ToolTipRole);
    triggerComboBox->setItemData(1, "A 5-seconds timer will initialize the capturing", Qt::ToolTipRole);

    // .. to the frequency ranges
    frequencyRangeComboBox->setItemData(0, "300 Hz up to 700 Hz", Qt::ToolTipRole);
    frequencyRangeComboBox->setItemData(1, "300 Hz up to 1500 Hz", Qt::ToolTipRole);
    frequencyRangeComboBox->setItemData(2, "100 Hz up to 20 kHz", Qt::ToolTipRole);

    // .. and finally to the tracking algorithms
    trackingAlgorithmComboBox->setItemData(0, "Recommended", Qt::ToolTipRole);
    trackingAlgorithmComboBox->setItemData(1, "Not recommended", Qt::ToolTipRole);

}

GUI::GUI() {    
    int argc = 0;
    applicationStart = false;
    
    QApplication app(argc,nullptr);

    QWidget window;
    window.setWindowTitle("Interactive Audio Visualizer");
    
    QVBoxLayout mainLayout;

    initializeComponents();

    initializeTexts();

    // Audio Settings
    QGroupBox audioSettings("Audio Settings");
    QVBoxLayout audioLayout;
    deviceComboBox->addItems(audioDevices);
    audioLayout.addWidget(audioDeviceLabel);
    audioLayout.addWidget(deviceComboBox);

    // Sample Rate ComboBox
    updateSampleRates(deviceComboBox->currentText());
    audioLayout.addWidget(sampleRateLabel);
    audioLayout.addWidget(sampleRateComboBox);

    QObject::connect(deviceComboBox, &QComboBox::currentTextChanged, 
    [this](const QString &text){
        updateSampleRates(text);
        updateNumChannelsInfo(text);
    });
    
    audioLayout.addWidget(createDropDownList(bufferSizeComboBox,bufferSizeLabel, {"32", "64", "128", "256", "512", "1024", "2048", "4096"}));
    audioLayout.addWidget(createDropDownList(quantizationComboBox,quantizationLabel, {QString::number( AudioHardware::quantizationRatio )} ));
    audioLayout.addLayout(numChannelsLayout);
    audioSettings.setLayout(&audioLayout);
    mainLayout.addWidget(&audioSettings);

    // Camera Settings
    QGroupBox cameraSettings("Camera Settings");
    QVBoxLayout cameraLayout;

    // Camera Device ComboBox
    cameraDeviceComboBox->addItems(cameraDevices);
    cameraLayout.addWidget(cameraDeviceLabel);
    cameraLayout.addWidget(cameraDeviceComboBox);

    // Resolution ComboBox
    updateResolution(cameraDeviceComboBox->currentText());
    cameraLayout.addWidget(cameraResolutionLabel);
    cameraLayout.addWidget(resolutionComboBox);

    QObject::connect(cameraDeviceComboBox, &QComboBox::currentTextChanged, 
        [this](const QString &text){
            updateResolution(text);
        });

    cameraLayout.addWidget(createDropDownList(frameRateComboBox,cameraFrameRateLabel, {"Auto"}));
    cameraSettings.setLayout(&cameraLayout);
    mainLayout.addWidget(&cameraSettings);

    // Display Settings
    QGroupBox displaySettings("Display Settings");
    QVBoxLayout displayLayout;
    displayLayout.addWidget(createDropDownList(displayResolutionComboBox,screenResolutionLabel, displayResolutions));
    displayLayout.addWidget(createDropDownList(displayFrameRateComboBox,screenFrameRateLabel, {"Auto"}));
    displaySettings.setLayout(&displayLayout);
    mainLayout.addWidget(&displaySettings);

    // IAV Settings
    QGroupBox iavSettings("IAV Settings");
    QVBoxLayout iavLayout;
    iavLayout.addWidget(createDropDownList(frequencyRangeComboBox,iavFrequencyRangeLabel, {"Narrow", "Normal", "Wide"}));
    iavLayout.addWidget(createDropDownList(roiComboBox,iavRegionOfInterestLabel, {"Small","Medium","Large"}));
    iavLayout.addWidget(createDropDownList(triggerComboBox,iavTriggerLabel, {"Manual", "Auto"}));
    iavLayout.addWidget(createDropDownList(trackingAlgorithmComboBox,iavTrackingAlgLabel, {"CSRT", "KCF"}));
    iavLayout.addWidget(createSkipFramesSlider(accuracyLabel, cpuLoadLabel));
    iavSettings.setLayout(&iavLayout);
    mainLayout.addWidget(&iavSettings);

    addExplanations();
    
    // Initialize errorLabel
    errorLabel = new QLabel();
    errorLabel->setStyleSheet("color: red;");
    errorLabel->setText("Camera resolution cannot exceed display resolution.");
    errorLabel->hide(); // Initially hidden
    mainLayout.addWidget(errorLabel);
    
    QPushButton startButton("Start");
        QObject::connect(&startButton, &QPushButton::clicked, [this]() {
            if(checkResolutionCompatibility()) {
                errorLabel->hide(); // Hide if resolutions are compatible
                saveCurrentStates();
                applicationStart = true;
                QApplication::quit();
                // exiting to start_iav();
            } else {
                errorLabel->show(); // Show error message if not compatible
            }
        });

    mainLayout.addWidget(&startButton);

    loadCurrentStates();
    
    window.setLayout(&mainLayout);
    window.show();

    QApplication::exec();
}

void GUI::initializeTexts(){
    
    // Get audio devices and sample rates supported
    std::vector<AudioHardware::Info> audio_hw_info;
    get_audio_hardware_info(audio_hw_info);
    for (const auto&[info,sr,nChannels]:audio_hw_info){
        // printf("%s : %d, %d\n",name.c_str(), sr.first, sr.second);
        QString audio_device = QString::fromStdString(info.first);
        audioExplanations.push_back(info.second);
        audioDevices.append(audio_device);
        numChannels.append(QString::number(nChannels));
        for (auto srate: AudioHardware::supportedRates){
            if (srate >= sr.first && srate <= sr.second){
                sampleRates[audio_device].append(QString::number(srate));
            }
        }
    }

    // Get available cameras and resolutions supported for each one of them
    auto cameras = getAvailableCameras();
    for (const auto& camera : cameras) {

        QString camera_device = QString::fromStdString(camera.devicePath);

        if (!camera.resolutions.empty()){
            cameraDevices.append(camera_device);
            for (const auto& res : camera.resolutions) {
                cameraResolutions[camera_device].append(QString::number(res.first) + "x" + QString::number(res.second));
            }
        }
    }

    // Get screen resolutions for the main screen
    auto screen_resolutions = get_screen_resolution();
    for(const auto&resolution : screen_resolutions){
        displayResolutions.append(QString::number(resolution.first) + "x" + QString::number(resolution.second));
    }

}

void GUI::saveCurrentStates(){

    std::unordered_map<std::string, std::string> settings;
    
    settings["audioDevice"] = deviceComboBox->currentText().toStdString();
    settings["sampleRate"] = sampleRateComboBox->currentText().toStdString();
    settings["cameraDevice"] = cameraDeviceComboBox->currentText().toStdString();
    settings["cameraResolution"] = resolutionComboBox->currentText().toStdString();
    settings["cameraFrameRate"] = std::to_string(approxFps);
    settings["bufferSize"] = bufferSizeComboBox->currentText().toStdString();
    settings["quantization"] = quantizationComboBox->currentText().toStdString();
    settings["numChannels"] = numOutputChannelsValue->text().toStdString();
    settings["quantizationRatio"] = std::to_string(AudioHardware::quantizationRatio);
    settings["frameRate"] = frameRateComboBox->currentText().toStdString();
    settings["displayResolution"] = displayResolutionComboBox->currentText().toStdString();
    settings["displayFrameRate"] = displayFrameRateComboBox->currentText().toStdString();
    settings["frequencyRange"] = frequencyRangeComboBox->currentText().toStdString();
    settings["roi"] = roiComboBox->currentText().toStdString();
    settings["trigger"] = triggerComboBox->currentText().toStdString();
    settings["trackingAlgorithm"] = trackingAlgorithmComboBox->currentText().toStdString();
    settings["skipFramesRatio"] = std::to_string(skipFramesSlider->value());

    settingsDB.saveSettings(settings);
}

void GUI::loadCurrentStates(){

    auto settings = settingsDB.loadSettings();
    
    if (audioDevices.contains(QString::fromStdString(settings["audioDevice"]))){
        deviceComboBox->setCurrentText(QString::fromStdString(settings["audioDevice"]));
    }
    if (sampleRates[QString::fromStdString(settings["audioDevice"])].contains(QString::fromStdString(settings["sampleRate"]))){
        sampleRateComboBox->setCurrentText(QString::fromStdString(settings["sampleRate"]));
    }
    if (cameraDevices.contains(QString::fromStdString(settings["cameraDevice"]))){
        cameraDeviceComboBox->setCurrentText(QString::fromStdString(settings["cameraDevice"]));
    }
    if (cameraResolutions[QString::fromStdString(settings["cameraDevice"])].contains(QString::fromStdString(settings["cameraResolution"]))){
        resolutionComboBox->setCurrentText(QString::fromStdString(settings["cameraResolution"]));
    }
    bufferSizeComboBox->setCurrentText(QString::fromStdString(settings["bufferSize"]));
    // numOutputChannelsValue->setText(QString::fromStdString(settings["numChannels"]));
    frameRateComboBox->setCurrentText(QString::fromStdString(settings["frameRate"]));
    if (displayResolutions.contains(QString::fromStdString(settings["displayResolution"]))){
        displayResolutionComboBox->setCurrentText(QString::fromStdString(settings["displayResolution"]));
    }
    displayFrameRateComboBox->setCurrentText(QString::fromStdString(settings["displayFrameRate"]));
    frequencyRangeComboBox->setCurrentText(QString::fromStdString(settings["frequencyRange"]));
    roiComboBox->setCurrentText(QString::fromStdString(settings["roi"]));
    triggerComboBox->setCurrentText(QString::fromStdString(settings["trigger"]));
    trackingAlgorithmComboBox->setCurrentText(QString::fromStdString(settings["trackingAlgorithm"]));
    skipFramesSlider->setValue(std::stoi(settings["skipFramesRatio"]));

}

void GUI::updateSampleRates(const QString &audioDevice) {
    sampleRateComboBox->clear();
    sampleRateComboBox->addItems(sampleRates[audioDevice]);
}

// static int lala = 1;
void GUI::updateNumChannelsInfo(const QString &audioDevice) {
    numOutputChannelsValue->setText(numChannels[audioDevices.indexOf(audioDevice)]);
}

void GUI::updateResolution(const QString &cameraDevice) {
    resolutionComboBox->clear();
    resolutionComboBox->addItems(cameraResolutions[cameraDevice]);
}

QWidget* GUI::createDropDownList(QComboBox *comboBox,QLabel *label,const QStringList& comboBoxItems) {
    
    auto *rowLayout = new QHBoxLayout;
    comboBox->addItems(comboBoxItems);

    rowLayout->addWidget(label);
    rowLayout->addWidget(comboBox);

    auto *rowWidget = new QWidget;
    rowWidget->setLayout(rowLayout);

    return rowWidget;
}

QWidget* GUI::createSkipFramesSlider(QLabel *label1, QLabel *label2) {

    skipFramesSlider->setMinimum(1); 
    skipFramesSlider->setMaximum(5);
    skipFramesSlider->setValue(0); 

    QWidget* sliderWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(sliderWidget);

    layout->addWidget(label1, 0, Qt::AlignLeft);
    layout->addWidget(skipFramesSlider);
    layout->addWidget(label2, 0, Qt::AlignRight);

    // optional .. 
    // connect(skipFramesSlider, &QSlider::valueChanged, this, &Gui::onSliderValueChanged);

    sliderWidget->setLayout(layout);  // Set the layout to the container widget
    return sliderWidget;  // Return the widget containing the slider and labels
}

bool GUI::checkResolutionCompatibility() {
    // Resolutions are currently stored as "WidthxHeight"
    QString cameraRes = resolutionComboBox->currentText();
    QString displayRes = displayResolutionComboBox->currentText();

    int cameraWidth = cameraRes.split("x")[0].toInt();
    int cameraHeight = cameraRes.split("x")[1].toInt();
    int displayWidth = displayRes.split("x")[0].toInt();
    int displayHeight = displayRes.split("x")[1].toInt();

    return (cameraWidth <= displayWidth && cameraHeight <= displayHeight);
}

bool GUI::onExit(){
    return !applicationStart;
}