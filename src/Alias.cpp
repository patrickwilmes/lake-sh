#include "Alias.hpp"
#include <stdexcept>

void LakeShell::AliasContainer::add(std::string name, std::string origin)
{
    if (exists(name)) {
        return;
    }
    m_aliases.emplace(name, origin);
}

bool LakeShell::AliasContainer::exists(std::string& name)
{
    return m_aliases.find(name) != m_aliases.end();
}

std::string LakeShell::AliasContainer::get_origin(std::string& name)
{
    if (!exists(name)) {
        throw std::runtime_error("alias doesn't exist!");
    }
    return m_aliases.at(name);
}
