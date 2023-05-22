#ifndef LAKE_SH_WORKINGDIRECTORY_HPP
#define LAKE_SH_WORKINGDIRECTORY_HPP

#include <string>

namespace LakeShell {
    class WorkingDirectory {
    public:
        explicit WorkingDirectory(std::string wd);
        std::string& get_working_dir();
        [[nodiscard]] bool is_git_dir() const;
        void refresh(std::string wd);
    private:
        void refresh_wd(std::string wd);
    private:
        std::string m_current_wd{};
        bool m_is_git_dir = false;
    };
}

#endif // LAKE_SH_WORKINGDIRECTORY_HPP
