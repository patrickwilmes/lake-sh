#include "ConfigStore.h"

#include <utility>

void LibConfig::ConfigStore::set(const std::string& key, std::string value)
{
    m_values[key] = std::move(value);
}

std::string LibConfig::ConfigStore::get(const std::string& key)
{
    return m_values[key];
}
