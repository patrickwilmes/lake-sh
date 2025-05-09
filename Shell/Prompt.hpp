#ifndef LAKE_SH_PROMPT_HPP
#define LAKE_SH_PROMPT_HPP

#include "ShellContext.hpp"
#include <memory>

#include <unordered_map>

namespace LakeShell {
    class Prompt {
    public:
        explicit Prompt(std::shared_ptr<ShellContext>  context);
        [[nodiscard]] std::string render_prompt() const;
    private:
        std::shared_ptr<ShellContext> m_shell_context;
        static std::string replace_template_vars(std::string templ, const std::unordered_map<std::string, std::string>& values);
    };
}

#endif // LAKE_SH_PROMPT_HPP
