#include "tools.h"

#include <cerrno>
#include <filesystem>

std::pair<std::vector<char>, int> readFile(const std::string& filename) {
    std::error_code ec;
    if (std::filesystem::is_directory(filename, ec)) {
        return {{}, -EISDIR};
    }

    errno = 0;
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        return {{}, errno ? -errno : -EIO};
    }

    std::streamoff fileSize = file.tellg();
    if (fileSize < 0) {
        return {{}, -EIO};
    }

    std::vector<char> buffer(static_cast<size_t>(fileSize));

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    if (file.gcount() != fileSize) {
        return {{}, -EIO};
    }
    file.close();

    return {std::move(buffer), 0};
}

std::pair<std::string, int> readFile_S(const std::string& filename) {
    std::error_code ec;
    if (std::filesystem::is_directory(filename, ec)) {
        return {{}, -EISDIR};
    }

    errno = 0;
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        return {{}, errno ? -errno : -EIO};
    }

    std::streamoff fileSize = file.tellg();
    if (fileSize < 0) {
        return {{}, -EIO};
    }

    std::string buffer(static_cast<size_t>(fileSize), '\0');

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    if (file.gcount() != fileSize) {
        return {{}, -EIO};
    }
    file.close();

    return {std::move(buffer), 0};
}
