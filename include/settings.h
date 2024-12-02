#ifndef SETTINGS_H
#define SETTINGS_H

#include <unordered_map>
#include <sqlite3.h>
#include "paths.h"

class SettingsDB {
public:

    SettingsDB(const std::string& db_path=Paths::databasePath);
    bool saveSettings(const std::unordered_map<std::string, std::string>& settings);
    std::unordered_map<std::string, std::string> loadSettings();

private:
    sqlite3* db;
    std::string dbPath;
};


#endif