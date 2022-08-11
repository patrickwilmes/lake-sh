#ifndef LAKE_SH_CMDHANDLER_HPP
#define LAKE_SH_CMDHANDLER_HPP

#include "Cmd.hpp"
#include "ShellContext.hpp"
#include <memory>
#include <vector>

namespace LakeShell::Cmd {
    using namespace LakeShell;
    class CommandHandler final {
    public:
        explicit CommandHandler(std::shared_ptr<ShellContext>& shell_context);
        void handle_commands(const std::shared_ptr<LakeShell::Cmd::CommandContainer>& cmd_container);

    private:

        bool external_cmd_exists(const std::shared_ptr<LakeShell::Cmd::Command>& cmd);
        void validate_external_commands(const std::vector<std::shared_ptr<LakeShell::Cmd::Command>>& cmds);

        void index_path();
        std::vector<std::shared_ptr<LakeShell::Cmd::Command>> resolve_aliased_commands(std::vector<std::shared_ptr<LakeShell::Cmd::Command>> cmds);

    private:
        std::vector<std::string> m_available_commands;
        std::shared_ptr<ShellContext> m_shell_context;
    };
}

#endif //LAKE_SH_CMDHANDLER_HPP
