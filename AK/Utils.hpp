#ifndef LC3_VM_UTILS_HPP
#define LC3_VM_UTILS_HPP

#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

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

static inline std::string left_trim(const std::string& str) {
    const auto start = str.find_first_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : str.substr(start);
}

static inline std::string right_trim(const std::string& str) {
    const auto end = str.find_last_not_of(" \t\n\r\f\v");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

static inline std::string trim_copy(const std::string& str)
{
    return right_trim(left_trim(str));
}

#endif // LC3_VM_UTILS_HPP
