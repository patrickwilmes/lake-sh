#ifndef LAKE_SH_HISTORY_H
#define LAKE_SH_HISTORY_H

#include <string>
#include <memory>
#include <vector>

namespace LakeShell {
    class History final {
    public:
        explicit History(uint32_t history_limit);
        void add(std::string &entry);
        std::string next();
    private:
        ulong m_cpos = 0;
        const uint32_t m_history_limit;
        std::vector<std::string> m_history;
    };
}// namespace LakeShell

#endif//LAKE_SH_HISTORY_H
