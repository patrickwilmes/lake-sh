#ifndef LAKE_SH_CMDHANDLER_HPP
#define LAKE_SH_CMDHANDLER_HPP

#include "Cmd.hpp"
#include "CmdAssembler.hpp"
#include "ShellContext.hpp"
#include <memory>
#include <vector>

namespace LakeShell::Cmd {
    using namespace LakeShell;
    class CommandHandler final {
    public:
        explicit CommandHandler(std::shared_ptr<ShellContext>& shell_context);
        std::string handle_commands(std::vector<std::shared_ptr<LakeShell::Cmd::Command>> cmds);

    private:
        bool is_own_cmd(const std::shared_ptr<LakeShell::Cmd::Command>& cmd);
        std::string handle_own_cmd(const std::shared_ptr<LakeShell::Cmd::Command>& cmd);

        bool external_cmd_exists(std::shared_ptr<LakeShell::Cmd::Command> cmd);
        void validate_external_commands(std::vector<std::shared_ptr<LakeShell::Cmd::Command>> cmds);

        void index_path();
        std::vector<std::shared_ptr<LakeShell::Cmd::Command>> resolve_aliased_commands(std::vector<std::shared_ptr<LakeShell::Cmd::Command>> cmds);

    private:
        std::vector<std::string> m_available_commands;
        std::shared_ptr<ShellContext> m_shell_context;
        static constexpr std::string_view PWD = "pwd";
        static constexpr std::string_view CD = "cd";
        static constexpr std::string_view ALIAS = "alias";
        static constexpr std::string_view EXPORT = "export";
        static constexpr std::string_view UNEXPORT = "unexport";
        static constexpr std::string_view ECHO = "echo";
        const std::vector<std::string_view> m_own_cmds = {
            PWD, CD, ALIAS, EXPORT, UNEXPORT, ECHO
        };
    };
}

#endif //LAKE_SH_CMDHANDLER_HPP
