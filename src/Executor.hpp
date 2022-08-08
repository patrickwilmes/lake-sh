#ifndef LAKE_SH_EXECUTOR_HPP
#define LAKE_SH_EXECUTOR_HPP

#include "Cmd.hpp"
#include "ShellContext.hpp"
#include <memory>
#include <string>
#include <vector>

namespace LakeShell {

    class Executor {
    public:
        Executor() = default;
        explicit Executor(std::shared_ptr<LakeShell::Cmd::Command> cmd);
        //FIXME: we dont need to return a string from execution anymore as the command itself should render the output
        virtual std::string execute();

    private:
        std::shared_ptr<LakeShell::Cmd::Command> m_cmd;
    };

    class OwnCommandExecutor final : public Executor {
    public:
        OwnCommandExecutor(std::shared_ptr<LakeShell::Cmd::Command> cmd, std::shared_ptr<LakeShell::ShellContext> shell_context);
        virtual std::string execute() override;
    private:
        std::shared_ptr<LakeShell::ShellContext> m_shell_context;
        static constexpr std::string_view PWD = "pwd";
        static constexpr std::string_view CD = "cd";
        static constexpr std::string_view ALIAS = "alias";
        static constexpr std::string_view EXPORT = "export";
        static constexpr std::string_view UNEXPORT = "unexport";
        static constexpr std::string_view ECHO = "echo";
        const std::vector<std::string_view> m_own_cmds = {
            PWD, CD, ALIAS, EXPORT, UNEXPORT, ECHO
        };
        std::shared_ptr<LakeShell::Cmd::Command> m_cmd;
    };

    class PipedExecutor final : public Executor {
    public:
        explicit PipedExecutor(std::vector<std::shared_ptr<LakeShell::Cmd::Command>> cmds);
        std::string execute() override;

    private:
        int spawn_command(std::shared_ptr<LakeShell::Cmd::Command> cmd, int input, bool is_first, bool is_last);

    private:
        const std::vector<std::shared_ptr<LakeShell::Cmd::Command>> m_cmds;
    };

    std::shared_ptr<Executor> create_executor(const std::vector<std::shared_ptr<LakeShell::Cmd::Command>> &commands, const std::shared_ptr<LakeShell::ShellContext> &ctx);
}

#endif //LAKE_SH_EXECUTOR_HPP
