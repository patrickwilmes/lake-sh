#include "Alias.hpp"

void LakeShell::AliasContainer::add(std::string name, std::string origin) {
    if (exists(name)) {
        return;
    }
    m_aliases.emplace(name, origin);
}

bool LakeShell::AliasContainer::exists(std::string &name) {
    return m_aliases.find(name) != m_aliases.end();
}

std::string LakeShell::AliasContainer::get_origin(std::string &name) {
    if (!exists(name)) {
        return ""; // todo - raise an exception in this case
    }
    return m_aliases.at(name);
}
