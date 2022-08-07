#include "Prompt.hpp"
#include "TextRendering.hpp" 

#include <utility>
using namespace LakeShell;

Prompt::Prompt(std::shared_ptr<ShellContext> context)
    : m_shell_context(std::move(context))
{
}

std::string Prompt::render_prompt()
{
    std::string prompt = m_shell_context->get_username();
    auto user_home = m_shell_context->get_user_home();
    auto wd = m_shell_context->get_working_dir();
    prompt += " @ " + m_shell_context->get_working_dir();
    if (m_shell_context->is_git_dir()) {
        prompt += spaced(red("git"));
    }
    return prompt + " >> ";
}
