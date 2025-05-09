#pragma once

#include "ConfigStore.h"
#include <memory>
#include <string>

namespace LibConfig {
    class ShellConfig final {
    public:
        explicit ShellConfig(const std::string& config_file);
        ~ShellConfig() = default;
        std::string get(const std::string& key) const;
    private:
        std::unique_ptr<ConfigStore> m_config_store{};
    };
}