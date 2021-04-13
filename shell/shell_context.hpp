#ifndef LAKE_SH_SHELL_CONTEXT_HPP
#define LAKE_SH_SHELL_CONTEXT_HPP

#include <vector>
#include <string>

namespace lsh {
    class shell_context final {
    public:
        void refresh();
        std::string get_working_dir();
        std::string get_user_home();
        std::string get_username();
        std::string get_relative_working_dir();
    private:
        std::string m_current_wd;
        std::string m_user_home;
        std::string m_username;
        std::vector<std::string> m_current_dirs;
    };
}

#endif//LAKE_SH_SHELL_CONTEXT_HPP
