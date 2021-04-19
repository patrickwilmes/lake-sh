#ifndef LAKE_SH_EXECUTOR_HPP
#define LAKE_SH_EXECUTOR_HPP

#include "Cmd.hpp"
#include <memory>
#include <string>
#include <vector>

namespace LakeShell {
    class Executor {
    public:
        Executor() = default;
        explicit Executor(std::shared_ptr<LakeShell::Cmd::Command> cmd);
        virtual std::string execute();

    private:
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
}

#endif //LAKE_SH_EXECUTOR_HPP
