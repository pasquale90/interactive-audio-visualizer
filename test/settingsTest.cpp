#include "settings.h"
#include "paths.h"
#include <gtest/gtest.h>

// Test fixture for SettingsDB
class SettingsDBTest : public testing::Test {
protected:
    
    SettingsDB settings{PathsTest::databasePath};

    std::unordered_map<std::string, std::string> testDict = {
        {"these", "are"},
        {"the", "tests"},
        {"for", "settings"},
        {"lol","..."}  
    };
};

TEST_F(SettingsDBTest,saveFileTest) {
    ASSERT_TRUE(settings.saveSettings(testDict));
}

TEST_F(SettingsDBTest,loadSettingsTest) {

    ASSERT_TRUE(settings.saveSettings(testDict));
    
    std::unordered_map<std::string, std::string> data =  settings.loadSettings();

    ASSERT_TRUE(data.size()==testDict.size());

    bool mismatch {false};
    for (const auto&[key,value]:data){
        if (testDict.find(key) == testDict.end() || testDict[key]!=value)
            mismatch = true;
    }
    ASSERT_FALSE(mismatch);
}

TEST_F(SettingsDBTest, overwriteSettings) {
    
    ASSERT_TRUE(settings.saveSettings(testDict));

    // Overwrite new settings
    std::unordered_map<std::string, std::string> newSettings = {
        {"these", "arenot"},
        {"lol", "!!!"}
    };
    ASSERT_TRUE(settings.saveSettings(newSettings));

    auto data = settings.loadSettings();
    bool hasChanged = true;
    for(const auto&[key,val]:newSettings){
        if (data[key]!=val)
            hasChanged = false;
    }
    ASSERT_TRUE(hasChanged);
}