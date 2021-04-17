#ifndef LAKE_SH_EXECUTOR_HPP
#define LAKE_SH_EXECUTOR_HPP

#include "cmd.hpp"
#include <string>
#include <vector>
#include <memory>

namespace lsh {
    class piped_executor final {
    public:
        explicit piped_executor(std::vector<std::shared_ptr<lsh::cmd::command>> cmds);
        std::string execute();
    private:
        int spawn_command(std::shared_ptr<lsh::cmd::command> cmd, int input, bool is_first, bool is_last);
    private:
        const std::vector<std::shared_ptr<lsh::cmd::command>> m_cmds;
    };
}

#endif//LAKE_SH_EXECUTOR_HPP
