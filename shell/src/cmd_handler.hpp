#ifndef LAKE_SH_CMD_HANDLER_HPP
#define LAKE_SH_CMD_HANDLER_HPP

#include "cmd.hpp"
#include "cmd_assembler.hpp"
#include "shell_context.hpp"
#include <memory>
#include <vector>

namespace lsh::cmd {
    using namespace lsh;
    class command_handler final {
    public:
        explicit command_handler(std::shared_ptr<shell_context> &shell_context);
        std::string handle_commands(std::vector<std::shared_ptr<lsh::cmd::command>> cmds);
    private:
        bool is_own_cmd(const std::shared_ptr<lsh::cmd::command> &cmd);
        std::string handle_own_cmd(const std::shared_ptr<lsh::cmd::command> &cmd);
        std::string handle_extern_cmds(const std::vector<std::shared_ptr<lsh::cmd::command>> &cmds);

        bool is_not_sync_command(std::shared_ptr<lsh::cmd::command> cmd);

        bool external_cmd_exists(std::shared_ptr<lsh::cmd::command> cmd);
        void validate_external_commands(std::vector<std::shared_ptr<lsh::cmd::command>> cmds);

        void index_path();
    private:
        std::vector<std::string> m_available_commands;
        std::shared_ptr<shell_context> m_shell_context;
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

#endif//LAKE_SH_CMD_HANDLER_HPP
