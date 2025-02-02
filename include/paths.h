#ifndef PATHS_H
#define PATHS_H

#include <string>
#include <filesystem>

/*!
 * @brief Function to get the absolute path of a given relative path.
 * @param relativePath - The path relative to the project's root directory.
 * @return std::string - The absolute path of the given relative path.
 * @note This function uses C++17's std::filesystem library.
 * @see https://en.cppreference.com/w/cpp/filesystem/path
*/
inline auto getAbsPath(const std::string& relativePath) -> std::string {
    std::filesystem::path rootDir = std::filesystem::absolute(__FILE__).parent_path();
    std::filesystem::path targetPath = rootDir / relativePath;
    return targetPath.lexically_normal().string();
}

/*!
 * @brief Namespace containing the path to settings.db file.
 * @see getAbsPath() function
*/
namespace Paths{
    const std::string databasePath {getAbsPath("../data/settings.db")};
}

/*!
 * @brief Namespace containing the path to the test.db file, used for testing purposes.
 * @see getAbsPath() function
 * @note These paths are different from the main paths used in the project.
 *       They are used for testing purposes only. The actual paths are defined in paths.h. 
*       This separation allows for easier management of paths when running tests. 
*/
namespace PathsTest{
    const std::string databasePath {getAbsPath("../data/test.db")};
}

#endif