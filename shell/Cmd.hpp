#ifndef LAKE_SH_CMD_HPP
#define LAKE_SH_CMD_HPP

#include <cstring>
#include <exception>
#include <string>
#include <vector>

namespace LakeShell::Cmd {
    class InvalidCommandException : public std::exception {
    public:
        explicit InvalidCommandException(const char * name): std::exception(), m_name(name) {}
        [[nodiscard]] const char * what() const noexcept override {
            return m_name;
        }
    private:
        const char *m_name;
    };

    class CommandNotFoundException : public std::exception {
    public:
        explicit CommandNotFoundException(const char * name): std::exception(), m_name(name) {}
        [[nodiscard]] const char * what() const noexcept override {
            return m_name;
        }
    private:
        const char *m_name;
    };

    class Command final {
    public:
        explicit Command(std::string name);
        void add_arg(const std::string& arg);
        void ensure_has_args(const uint32_t expected_number);
        std::string get_name();
        std::vector<std::string> get_args();
        uint32_t arg_count();
    private:
        const std::string m_name;
        std::vector<std::string> m_args;
    };
}

#endif//LAKE_SH_CMD_HPP
