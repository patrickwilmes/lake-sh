#pragma once

#include "ConfigStore.h"
#include "Token.h"
#include <vector>
#include <memory>

namespace LibConfig {
    class TomlParser final{
    public:
        explicit TomlParser(std::vector<Token> tokens);
        std::unique_ptr<ConfigStore> parse();

    private:
        std::vector<Token> m_tokens;
        std::unique_ptr<ConfigStore> m_config_store{};
    };
}