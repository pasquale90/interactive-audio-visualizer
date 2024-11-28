#ifndef PATHS_H
#define PATHS_H

#include <string>
#include <filesystem>
#include <iostream>

inline auto getAbsPath(const std::string& relativePath) -> std::string {
    std::filesystem::path rootDir = std::filesystem::absolute(__FILE__).parent_path();
    std::cout<<rootDir<<std::endl;
    std::filesystem::path targetPath = rootDir / relativePath;
    return targetPath.lexically_normal().string();
}

namespace Paths{
    const std::string databasePath {getAbsPath("../data/settings.db")};
}


#endif