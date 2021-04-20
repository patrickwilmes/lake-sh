#ifndef LAKE_SH_ALIAS_HPP
#define LAKE_SH_ALIAS_HPP

#include <map>
#include <string>

namespace LakeShell {
    class AliasContainer final {
    public:
        void add(std::string name, std::string origin);
        bool exists(std::string& name);
        std::string get_origin(std::string& name);

    private:
        std::map<std::string, std::string> m_aliases;
    };
}

#endif //LAKE_SH_ALIAS_HPP
