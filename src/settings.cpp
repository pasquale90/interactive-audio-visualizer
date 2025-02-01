#include "sqlite3.h"
#include "settings.h"
#include "config.h"

SettingsDB::SettingsDB(const std::string& db_path) : dbPath(db_path) {
   
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        db = nullptr;
        return;
    }
    
    // Create table if it doesn't exist
    const char* createTableSQL = 
        "CREATE TABLE IF NOT EXISTS settings ("
        "setting_name TEXT PRIMARY KEY,"
        "setting_value TEXT NOT NULL);";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        sqlite3_free(errMsg);
        sqlite3_close(db);
        db = nullptr;
    }
}

SettingsDB::~SettingsDB(){

    if (db) {
        sqlite3_close(db);
    }
    
}

bool SettingsDB::saveSettings(const std::unordered_map<std::string, std::string>& settings) {
    if (!db) return false;

    // Begin transaction for better performance
    char* errMsg = nullptr;
    if (sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, &errMsg) != SQLITE_OK) {
        sqlite3_free(errMsg);
        return false;
    }

    // First, clear existing settings
    const char* clearSQL = "DELETE FROM settings;";
    if (sqlite3_exec(db, clearSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        sqlite3_free(errMsg);
        return false;
    }

    // Prepare the insert statement
    sqlite3_stmt* stmt;
    const char* insertSQL = "INSERT INTO settings (setting_name, setting_value) VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    // Insert all settings
    for (const auto& [key, value] : settings) {
        sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, value.c_str(), -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_reset(stmt);
    }

    sqlite3_finalize(stmt);

    if (sqlite3_exec(db, "COMMIT", nullptr, nullptr, &errMsg) != SQLITE_OK) {
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

std::unordered_map<std::string, std::string> SettingsDB::loadSettings() {
    std::unordered_map<std::string, std::string> settings;
    
    if (!db) return settings;

    const char* selectSQL = "SELECT setting_name, setting_value FROM settings;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        return settings;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        settings[name] = value;
    }

    sqlite3_finalize(stmt);
    return settings;
}