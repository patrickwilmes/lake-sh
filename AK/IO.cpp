#include "IO.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string AK::IO::read_file(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << path << std::endl;
        return {};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}