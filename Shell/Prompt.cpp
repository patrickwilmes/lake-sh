#include "Prompt.hpp"
#include "TextRendering.hpp"

#include <regex>
#include <utility>
using namespace LakeShell;

Prompt::Prompt(std::shared_ptr<ShellContext> context)
    : m_shell_context(std::move(context))
{
}

std::string Prompt::render_prompt() const
{
    auto template_string = m_shell_context->get_config("template");
    std::unordered_map<std::string, std::string> values = {
        { "user", m_shell_context->get_username() },
        { "home", m_shell_context->get_user_home() },
        { "git", m_shell_context->is_git_dir() ? "git" : "" },
        { "git_branch", "main" },
        { "time", "none" },
        { "prompt_sign", ">>\n%" }
    };
    return replace_template_vars(template_string, values);
}

std::string Prompt::replace_template_vars(std::string templ, const std::unordered_map<std::string, std::string>& values)
{
    std::string result = std::move(templ);
    for (const auto& [key, value] : values) {
        std::string pattern = "\\{" + key + "\\}";
        result = std::regex_replace(result, std::regex(pattern), value);
    }
    return result;
}
