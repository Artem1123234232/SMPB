#include "tools.h"

std::pair<std::vector<char>, int> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        return {{}, ENOENT};
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return {buffer, 0};
}

std::pair<std::string, int> readFile_S(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        return {{}, ENOENT};
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::string buffer(fileSize, '\0');

    file.seekg(0);
    file.read(&buffer[0], fileSize);
    file.close();

    return {buffer, 0};
}
