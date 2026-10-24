#pragma once

// For read files
#include <fstream>

// For vectors
#include <vector>

// For strings
#include <string>

std::pair<std::vector<char>, int> readFile(const std::string& filename);

std::pair<std::string, int> readFile_S(const std::string& filename);