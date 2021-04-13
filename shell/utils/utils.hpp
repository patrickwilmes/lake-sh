#ifndef LC3_VM_UTILS_HPP
#define LC3_VM_UTILS_HPP

#include <string>

inline uint16_t swap16(uint16_t x) { return (x << 8) | (x >> 8); }

static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch){
    return !std::isspace(ch);
  }).base(), s.end());
}

static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

#endif // LC3_VM_UTILS_HPP
