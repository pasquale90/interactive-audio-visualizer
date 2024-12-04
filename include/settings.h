#ifndef SETTINGS_H
#define SETTINGS_H

#include <unordered_map>
#include <sqlite3.h>
#include "config.h"
#include "paths.h"

class SettingsDB {
public:

    explicit SettingsDB(const std::string& db_path=Paths::databasePath);
    bool saveSettings(const std::unordered_map<std::string, std::string>& settings);
    std::unordered_map<std::string, std::string> loadSettings();

    // define Destructor to deallocate sql resources
    ~SettingsDB();
    // complying with the rule of 5 to prevent unintented copying or moving.
    SettingsDB(const SettingsDB&) = delete; // delete copy constructor
    SettingsDB& operator=(const SettingsDB&) = delete; // delete copy assignment operator
    SettingsDB(SettingsDB&&) = delete; // delete move constructor
    SettingsDB& operator=(SettingsDB&&) = delete; //  delete move assignement operator

private:
    sqlite3* db;
    std::string dbPath;
};


#endif