#include "alias.hpp"

void lsh::alias_container::add(std::string name, std::string origin) {
    if (exists(name)) {
        return;
    }
    m_aliases.emplace(name, origin);
}

bool lsh::alias_container::exists(std::string &name) {
    return m_aliases.find(name) != m_aliases.end();
}

std::string lsh::alias_container::get_origin(std::string &name) {
    if (!exists(name)) {
        return ""; // todo - raise an exception in this case
    }
    return m_aliases.at(name);
}
