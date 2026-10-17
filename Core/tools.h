#pragma once

// For read files
#include <fstream>

// For vectors
#include <vector>

std::pair<std::vector<char>, int> readFile(const std::string& filename);