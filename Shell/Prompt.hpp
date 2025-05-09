#ifndef LAKE_SH_PROMPT_HPP
#define LAKE_SH_PROMPT_HPP

#include <memory>
#include "ShellContext.hpp"

namespace LakeShell {
    class Prompt {
    public:
        explicit Prompt(std::shared_ptr<ShellContext>  context);
        std::string render_prompt() const;
    private:
        std::shared_ptr<ShellContext> m_shell_context;
    };
}

#endif // LAKE_SH_PROMPT_HPP
