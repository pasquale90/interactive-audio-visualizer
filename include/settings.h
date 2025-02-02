#ifndef SETTINGS_H
#define SETTINGS_H

#include <unordered_map>
#include "paths.h"
class sqlite3;

/*!
 * @brief Class to manage settings using a SQLite database.
*/
class SettingsDB {
public:

    /*!
     * @brief Constructor to initialize the database connection.
     * @param db_path - The path to the SQLite database file.
     * @note If the database file does not exist, it will be created.
     */
    explicit SettingsDB(const std::string& db_path=Paths::databasePath);
    
    /*!
     * @brief Function to save the given settings to the database.
     * @param const std::unordered_map<std::string, std::string>& settings - The map of settings to be saved.
     * @return true if the settings were successfully saved, false otherwise.
    */
    bool saveSettings(const std::unordered_map<std::string, std::string>& settings);
    
    /*!
     * @brief Function to load the settings from the database.
     * @return std::unordered_map<std::string, std::string> - The loaded settings.
    */
    std::unordered_map<std::string, std::string> loadSettings();

    /*!
     * @brief Destructor to close the database connection and deallocate sql resources.
    */
    ~SettingsDB();
    
    
    // complying with the rule of 5 to prevent unintented copying or moving.

    /*!
     * @brief Copy constructor is deleted to prevent accidental use.
    */
    SettingsDB(const SettingsDB&) = delete; // delete copy constructor
    
    /*!
     * @brief Copy assignment operator is deleted to prevent accidental use.
    */
    SettingsDB& operator=(const SettingsDB&) = delete; // delete copy assignment operator
    
    /*!
     * @brief Move constructor is deleted to prevent accidental use.
    */
    SettingsDB(SettingsDB&&) = delete; // delete move constructor
    
    /*!
     * @brief Move assignment operator is deleted to prevent accidental use.
    */
    SettingsDB& operator=(SettingsDB&&) = delete; //  delete move assignement operator

private:
    sqlite3* db;
    std::string dbPath;
};


#endif