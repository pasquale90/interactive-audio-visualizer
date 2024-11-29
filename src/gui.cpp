#include "gui.h"
#include <cstddef>
#include <string>

// forward declaration of function defined in main.cpp
void start_iav();

#include "audiohw.h"
#include "camerahw.h"
#include "screenhw.h"

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
    
    audioDeviceLabel = new QLabel("Device:");
    sampleRateLabel = new QLabel("Sample Rate:");
    cameraDeviceLabel = new QLabel("Device:");
    cameraResolutionLabel = new QLabel("Resolution:");
    bufferSizeLabel = new QLabel("Buffer Size");
    quantizationLabel = new QLabel("Quantization");
    cameraFrameRateLabel = new QLabel("Frame Rate");
    screenResolutionLabel= new QLabel("Resolution");
    screenFrameRateLabel= new QLabel("Frames per second");
    iavFrequencyRangeLabel= new QLabel("Frequency range");
    iavRegionOfInterestLabel= new QLabel("ROI");
    iavTriggerLabel= new QLabel("Trigger");
    iavTrackingAlgLabel= new QLabel("Tracking algorithm");

    audioDeviceLabel->setToolTip("Choose audio output device.");
    sampleRateLabel->setToolTip("Set samples per second.");
    cameraDeviceLabel->setToolTip("Select camera input device.");
    cameraResolutionLabel->setToolTip("Set camera capture resolution.");
    bufferSizeLabel->setToolTip("Configure audio buffer size.");
    quantizationLabel->setToolTip("Quantization range for digitalizing audio data");
    cameraFrameRateLabel->setToolTip("Set camera frame rate (currently fixed).");
    screenResolutionLabel->setToolTip("Select screen resolution.");
    screenFrameRateLabel->setToolTip("Set screen frame rate (currently fixed).");
    iavFrequencyRangeLabel->setToolTip("Define audio frequency range for sound generation");
    iavRegionOfInterestLabel->setToolTip("Set region of interest size (window size for photshooting pattern).");
    iavTriggerLabel->setToolTip("Choose capturing method.");
    iavTrackingAlgLabel->setToolTip("Select object tracking algorithm.");

}


void GUI::addExplanations(){

    // Add explanation to the audio devices,
    for (size_t i = 0; i<audioExplanations.size(); ++i)
        deviceComboBox->setItemData(i, QString::fromStdString( audioExplanations[i] ), Qt::ToolTipRole);

    // .. to the frame rates of both camera and screen,
    frameRateComboBox->setItemData(0, "≈25-30 fps", Qt::ToolTipRole);
    displayFrameRateComboBox->setItemData(0, "≈25-30 fps", Qt::ToolTipRole);

    // ... to the roi comboBox options,
    roiComboBox->setItemData(0, "5% of camera's capture resolution.", Qt::ToolTipRole);
    roiComboBox->setItemData(1, "10% of camera's capture resolution.", Qt::ToolTipRole);
    roiComboBox->setItemData(2, "15% of camera's capture resolution.", Qt::ToolTipRole);

    // .. to the capturing method,
    triggerComboBox->setItemData(0, "Capturing is initialized manually, using the Space Bar key", Qt::ToolTipRole);
    triggerComboBox->setItemData(1, "A 5-seconds timer will initialize the capturing", Qt::ToolTipRole);

    // .. and finally to the frequency ranges
    frequencyRangeComboBox->setItemData(0, "300 Hz up to 700 Hz", Qt::ToolTipRole);
    frequencyRangeComboBox->setItemData(1, "300 Hz up to 1500 Hz", Qt::ToolTipRole);
    frequencyRangeComboBox->setItemData(2, "100 Hz up to 20 kHz", Qt::ToolTipRole);

}

GUI::GUI(int argc, char *argv[]) {
    
    QApplication app(argc,argv);

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
    });
    
    audioLayout.addWidget(createDropDownList(bufferSizeComboBox,bufferSizeLabel, {"32", "64", "128", "256", "512", "1024", "2048", "4096"}));
    audioLayout.addWidget(createDropDownList(quantizationComboBox,quantizationLabel, {QString::number( quantizationRatio )} ));
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
    iavLayout.addWidget(createDropDownList(triggerComboBox,iavTriggerLabel, {"Manual (Space Bar)", "Auto (Timer - 5 sec)"}));
    iavLayout.addWidget(createDropDownList(trackingAlgorithmComboBox,iavTrackingAlgLabel, {"CSRT", "KFC", "BOOSTING"}));
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
        QObject::connect(&startButton, &QPushButton::clicked, [this, &app]() {
            if(checkResolutionCompatibility()) {
                errorLabel->hide(); // Hide if resolutions are compatible
                saveCurrentStates();
                app.quit();
                start_iav();
            } else {
                errorLabel->show(); // Show error message if not compatible
            }
        });

    mainLayout.addWidget(&startButton);

    loadCurrentStates();
    
    window.setLayout(&mainLayout);
    window.show();

    app.exec();
}

void GUI::initializeTexts(){
    
    // Get audio devices and sample rates supported
    AHI audio_hw_info;
    get_audio_hardware_info(audio_hw_info);
    for (const auto&[info,sr]:audio_hw_info){
        // printf("%s : %d, %d\n",name.c_str(), sr.first, sr.second);
        QString audio_device = QString::fromStdString(info.first);
        audioExplanations.push_back(info.second);
        audioDevices.append(audio_device);
        for (size_t i = 0; i< supportedRates.size(); ++i){
            if (supportedRates[i] >= sr.first && supportedRates[i]<= sr.second)
                sampleRates[audio_device].append(QString::number(supportedRates[i]));
        }
    }

    // Get available cameras and resolutions supported for each one of them
    auto cameras = getAvailableCameras();
    for (const auto& camera : cameras) {

        QString camera_device = QString::fromStdString(camera.devicePath);

        if (!camera.resolutions.empty()){
            cameraDevices.append(camera_device);
            for (const auto& res : camera.resolutions) 
                cameraResolutions[camera_device].append(QString::number(res.first) + "x" + QString::number(res.second));
        }
    }

    // Get screen resolutions for the main screen
    auto screen_resolutions = get_screen_resolution();
    for(const std::pair&[width,height] : screen_resolutions){
        displayResolutions.append(QString::number(width) + "x" + QString::number(height));
    }

}


void GUI::saveCurrentStates(){

    std::unordered_map<std::string, std::string> settings;
    
    settings["audioDevice"] = deviceComboBox->currentText().toStdString();
    settings["sampleRate"] = sampleRateComboBox->currentText().toStdString();
    settings["cameraDevice"] = cameraDeviceComboBox->currentText().toStdString();
    settings["cameraResolution"] = resolutionComboBox->currentText().toStdString();
    settings["bufferSize"] = bufferSizeComboBox->currentText().toStdString();
    settings["frameRate"] = frameRateComboBox->currentText().toStdString();
    settings["displayResolution"] = displayResolutionComboBox->currentText().toStdString();
    settings["displayFrameRate"] = displayFrameRateComboBox->currentText().toStdString();
    settings["frequencyRange"] = frequencyRangeComboBox->currentText().toStdString();
    settings["roi"] = roiComboBox->currentText().toStdString();
    settings["trigger"] = triggerComboBox->currentText().toStdString();
    settings["trackingAlgorithm"] = trackingAlgorithmComboBox->currentText().toStdString();

    settingsDB.saveSettings(settings);
}

void GUI::loadCurrentStates(){

    if (!settingsDB.isFirstRun()) {
        auto settings = settingsDB.loadSettings();
        
        if (audioDevices.contains(QString::fromStdString(settings["audioDevice"])))
            deviceComboBox->setCurrentText(QString::fromStdString(settings["audioDevice"]));
        if (sampleRates[QString::fromStdString(settings["audioDevice"])].contains(QString::fromStdString(settings["sampleRate"])))
            sampleRateComboBox->setCurrentText(QString::fromStdString(settings["sampleRate"]));
        if (cameraDevices.contains(QString::fromStdString(settings["cameraDevice"])))
            cameraDeviceComboBox->setCurrentText(QString::fromStdString(settings["cameraDevice"]));
        if (cameraResolutions[QString::fromStdString(settings["cameraDevice"])].contains(QString::fromStdString(settings["cameraResolution"])))
            resolutionComboBox->setCurrentText(QString::fromStdString(settings["cameraResolution"]));
        bufferSizeComboBox->setCurrentText(QString::fromStdString(settings["bufferSize"]));
        frameRateComboBox->setCurrentText(QString::fromStdString(settings["frameRate"]));
        if (displayResolutions.contains(QString::fromStdString(settings["displayResolution"])))
            displayResolutionComboBox->setCurrentText(QString::fromStdString(settings["displayResolution"]));
        displayFrameRateComboBox->setCurrentText(QString::fromStdString(settings["displayFrameRate"]));
        frequencyRangeComboBox->setCurrentText(QString::fromStdString(settings["frequencyRange"]));
        roiComboBox->setCurrentText(QString::fromStdString(settings["roi"]));
        triggerComboBox->setCurrentText(QString::fromStdString(settings["trigger"]));
        trackingAlgorithmComboBox->setCurrentText(QString::fromStdString(settings["trackingAlgorithm"]));
    }

}

void GUI::updateSampleRates(const QString &audioDevice) {
    sampleRateComboBox->clear();
    sampleRateComboBox->addItems(sampleRates[audioDevice]);
}

void GUI::updateResolution(const QString &cameraDevice) {
    resolutionComboBox->clear();
    resolutionComboBox->addItems(cameraResolutions[cameraDevice]);
}

QWidget* GUI::createDropDownList(QComboBox *comboBox,QLabel *label,const QStringList& comboBoxItems) {
    
    QHBoxLayout *rowLayout = new QHBoxLayout;
    comboBox->addItems(comboBoxItems);

    rowLayout->addWidget(label);
    rowLayout->addWidget(comboBox);

    QWidget *rowWidget = new QWidget;
    rowWidget->setLayout(rowLayout);

    return rowWidget;
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