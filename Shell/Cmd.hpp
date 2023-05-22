#ifndef LAKE_SH_CMD_HPP
#define LAKE_SH_CMD_HPP

#include <cstring>
#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace LakeShell::Cmd {
    class InvalidCommandException : public std::exception {
    public:
        explicit InvalidCommandException(const char* name)
            : std::exception()
            , m_name(name)
        {
        }
        [[nodiscard]] const char* what() const noexcept override
        {
            return m_name;
        }

    private:
        const char* m_name;
    };

    class CommandNotFoundException : public std::exception {
    public:
        explicit CommandNotFoundException(std::string name)
            : std::exception()
            , m_name(std::move(name))
        {
        }
        [[nodiscard]] const char* what() const noexcept override
        {
            return m_name.c_str();
        }

    private:
        std::string m_name;
    };

    class Command final {
    public:
        explicit Command(std::string name);
        void add_arg(const std::string& arg);
        void ensure_has_args(uint32_t expected_number);
        std::string get_name();
        std::string assemble_alias();
        std::vector<std::string> get_args();
        uint32_t arg_count();
        bool is_internal_command();

    private:
        static constexpr std::string_view PWD = "pwd";
        static constexpr std::string_view CD = "cd";
        static constexpr std::string_view ALIAS = "alias";
        static constexpr std::string_view EXPORT = "export";
        static constexpr std::string_view UNEXPORT = "unexport";
        static constexpr std::string_view ECHO = "echo";
        const std::vector<std::string_view> m_own_cmds = {
            PWD, CD, ALIAS, EXPORT, UNEXPORT, ECHO
        };
        std::string m_name;
        std::vector<std::string> m_args;
    };

    class CommandContainer final {
    public:
        CommandContainer() = default;
        CommandContainer(const std::vector<std::shared_ptr<LakeShell::Cmd::Command>>& cmds, bool is_concat);
        [[nodiscard]] bool is_concat() const;
        std::vector<std::shared_ptr<Command>> get_commands();

    private:
        std::vector<std::shared_ptr<Command>> m_cmds;
        bool m_is_concat {};
    };

}

#endif // LAKE_SH_CMD_HPP
