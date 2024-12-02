#ifndef PATHS_H
#define PATHS_H

#include <string>
#include <filesystem>

inline auto getAbsPath(const std::string& relativePath) -> std::string {
    std::filesystem::path rootDir = std::filesystem::absolute(__FILE__).parent_path();
    std::filesystem::path targetPath = rootDir / relativePath;
    return targetPath.lexically_normal().string();
}

namespace Paths{
    const std::string databasePath {getAbsPath("../data/settings.db")};
}

namespace PathsTest{
    const std::string databasePath {getAbsPath("../data/test.db")};
}

#endif