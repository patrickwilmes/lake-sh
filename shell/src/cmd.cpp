#include "cmd.hpp"

#include <utility>

lsh::cmd::command::command(std::string name) : m_name(std::move(name)) {}

void lsh::cmd::command::add_arg(const std::string& arg) {
    m_args.push_back(arg);
}

void lsh::cmd::command::ensure_has_args(const uint32_t expected_number) {
    if (expected_number != m_args.size()) {
        throw invalid_command_exception(m_name.c_str());
    }
}

std::string lsh::cmd::command::get_name() {
    return m_name;
}

std::vector<std::string> lsh::cmd::command::get_args() {
    return m_args;
}
uint32_t lsh::cmd::command::arg_count() {
    return m_args.size();
}
