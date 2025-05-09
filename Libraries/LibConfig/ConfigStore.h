#pragma once

#include <map>
#include <string>

namespace LibConfig {
    class ConfigStore final {
    public:
        void set(const std::string& key, std::string value);
        std::string get(const std::string& key);
    private:
        std::map<std::string, std::string> m_values {};
    };
}