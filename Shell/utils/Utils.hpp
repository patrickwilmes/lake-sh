#ifndef LC3_VM_UTILS_HPP
#define LC3_VM_UTILS_HPP

#include <filesystem>
#include <algorithm>
#include <string>
#include <vector>

inline uint16_t swap16(uint16_t x) { return (x << 8) | (x >> 8); }

static inline void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

static inline void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(),
        s.end());
}

static inline void trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}

static inline std::vector<std::string> get_dirs_for(std::string& path)
{
    std::vector<std::string> entries;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_directory()) {
            entries.push_back(entry.path().string());
        }
    }
    return entries;
}

#endif // LC3_VM_UTILS_HPP
