#ifndef LAKE_SH_CMD_HANDLER_HPP
#define LAKE_SH_CMD_HANDLER_HPP

#include "cmd_assembler.hpp"
#include "shell_context.hpp"
#include <memory>
#include <vector>

namespace lsh::cmd {
    using namespace lsh;
    class command_handler final {
    public:
        explicit command_handler(std::shared_ptr<shell_context> &shell_context);
        std::string handle_commands(std::vector<std::shared_ptr<lsh::assembler::cmd>> cmds);
    private:
        bool is_own_cmd(const std::shared_ptr<lsh::assembler::cmd> &cmd);
        void handle_own_cmd(const std::shared_ptr<lsh::assembler::cmd> &cmd);
        std::string handle_extern_cmds(const std::vector<std::shared_ptr<lsh::assembler::cmd>> &cmds);

        bool is_not_sync_command(std::shared_ptr<lsh::assembler::cmd> cmd);

    private:
        std::shared_ptr<shell_context> m_shell_context;
    };
}

#endif//LAKE_SH_CMD_HANDLER_HPP
