#ifndef GUI_H
#define GUI_H

#include <QApplication>
#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QMap>
#include <QPair>

#include "settings.h"

class GUI{
public:

    GUI(int argc, char *argv[]);

private:
    
    void initializeComponents();
    void initializeTexts();
    void setupComboBoxes(QApplication);

    QWidget* createDropDownList(QComboBox *, QLabel *, const QStringList&);

    void updateSampleRates(const QString&);
    void updateResolution(const QString&);

    void saveCurrentStates();
    void loadCurrentStates();
    bool checkResolutionCompatibility();

    void addExplanations();

    SettingsDB settingsDB;

    QStringList audioDevices,
                cameraDevices,
                displayResolutions;
    
    QMap<QString,QStringList> sampleRates,cameraResolutions;

    QComboBox *deviceComboBox,
                *sampleRateComboBox,
                *cameraDeviceComboBox,
                *resolutionComboBox,
                *bufferSizeComboBox,
                *frameRateComboBox,
                *displayResolutionComboBox,
                *displayFrameRateComboBox,
                *frequencyRangeComboBox,
                *roiComboBox,
                *triggerComboBox,
                *trackingAlgorithmComboBox;

    QLabel *audioDeviceLabel,
            *sampleRateLabel,
            *cameraDeviceLabel,
            *cameraResolutionLabel,
            *bufferSizeLabel,
            *cameraFrameRateLabel,
            *screenResolutionLabel,
            *screenFrameRateLabel,
            *iavFrequencyRangeLabel,
            *iavRegionOfInterestLabel,
            *iavTriggerLabel,
            *iavTrackingAlgLabel,
            *errorLabel;
};

#endif
