
#include "History.hpp"

LakeShell::History::History(const uint32_t history_limit): m_history_limit(history_limit) {
    m_history.reserve(m_history_limit);
}

void LakeShell::History::add(std::string &entry) {
    m_history.emplace_back(entry);
    m_cpos = m_history.size() - 1;
}

std::string LakeShell::History::next() {
    auto elem = m_history[m_cpos];
    m_cpos--;
    return elem;
}
