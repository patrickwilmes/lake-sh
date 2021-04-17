
#include "history.hpp"

lsh::history::history(const uint32_t history_limit): m_history_limit(history_limit) {
    m_history.reserve(m_history_limit);
}

void lsh::history::add(std::string &entry) {
    m_history.emplace_back(entry);
    m_cpos = m_history.size() - 1;
}

std::string lsh::history::next() {
    auto elem = m_history[m_cpos];
    m_cpos--;
    return elem;
}
