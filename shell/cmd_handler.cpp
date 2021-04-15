#include "cmd_handler.hpp"
#include "shell_context.hpp"
#include "usr.hpp"
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>
#include <cassert>
#include <filesystem>

class piped_executor final {
public:
    explicit piped_executor(std::vector<std::shared_ptr<lsh::cmd::command>> cmds) : m_cmds(std::move(cmds)) {}

    std::string execute() {
        int input = 0;
        int num_cmds = m_cmds.size();
        for (int i = 0; i < num_cmds; i++) {
            input = spawn_command(m_cmds[i], input, i == 0, i == num_cmds - 1);
        }
        char buffer[2048];
        while (read(input, buffer, sizeof(buffer)) != 0)
            ;
        std::string data(buffer);
        close(input);
        return data;
    }

private:
    int spawn_command(std::shared_ptr<lsh::cmd::command> cmd, int input, bool is_first, bool is_last) {
        int pid;
        int fd[2];
        pipe(fd);

        pid = fork();

        if (pid == 0) {
            if (is_first) {
                dup2(fd[1], STDOUT_FILENO);
            } else if (is_last) {
                dup2(input, STDIN_FILENO);
                dup2(fd[1], STDOUT_FILENO);// exp
            } else {
                dup2(input, STDIN_FILENO);
                dup2(fd[1], STDOUT_FILENO);
            }
            auto cmd_name = cmd->get_name();
            const char *c = cmd_name.c_str();
            char *args[cmd->arg_count() + 2];// = {const_cast<char *>(c), const_cast<char *>(latr), NULL};
            args[0] = const_cast<char *>(c);
            int i = 1;
            for (auto &arg : cmd->get_args()) {
                args[i] = const_cast<char *>(arg.c_str());
                i++;
            }
            args[i] = NULL;
            if (execvp(cmd_name.c_str(), args) == -1) {
                exit(-1);
            }
        }

        if (input != 0) {
            close(input);
        }

        close(fd[1]);

        return fd[0];
    }

private:
    const std::vector<std::shared_ptr<lsh::cmd::command>> m_cmds;
};


std::string lsh::cmd::command_handler::handle_commands(std::vector<std::shared_ptr<lsh::cmd::command>> cmds) {
    auto copy_if_is_external_cmd = [&](const std::shared_ptr<lsh::cmd::command> &cmd) {
        return !is_own_cmd(cmd);
    };

    for (auto &cmd : cmds) {
        if (is_own_cmd(cmd)) {
            return handle_own_cmd(cmd);
        }
    }

    std::vector<std::shared_ptr<lsh::cmd::command>> extern_cmds;
    std::copy_if(cmds.begin(), cmds.end(), std::back_inserter(extern_cmds), copy_if_is_external_cmd);
    return handle_extern_cmds(extern_cmds);
}

lsh::cmd::command_handler::command_handler(std::shared_ptr<shell_context> &shell_context): m_shell_context(shell_context) {
    index_path();
}

bool lsh::cmd::command_handler::is_own_cmd(const std::shared_ptr<lsh::cmd::command> &cmd) {
    return std::find(m_own_cmds.begin(), m_own_cmds.end(), cmd->get_name()) != m_own_cmds.end();
}

std::string lsh::cmd::command_handler::handle_own_cmd(const std::shared_ptr<lsh::cmd::command> &cmd) {
    auto cmd_name = cmd->get_name();
    auto args = cmd->get_args();
    if (cmd_name == PWD) {
        std::string wd = lsh::usr::current_wd();
        return wd;
    } else if (cmd_name == CD) {
        if (args.empty()) {
            std::string usr_home = lsh::usr::usr_home_dir();
            chdir(usr_home.c_str());
        } else {
            cmd->ensure_has_args(1);
            auto resolve_path = [](std::string &origin_path) {
              std::string wd = lsh::usr::current_wd();
              if (origin_path == "..") {
                  return wd.substr(0, wd.rfind('/'));
              } else if ((origin_path[0] != '.' && origin_path[0] != '/') || (origin_path[0] == '.' && origin_path[1] == '/')) {
                  return wd + "/" + origin_path;
              }
              assert(origin_path[0] == '/');
              return origin_path;
            };
            std::string path_string = resolve_path(args[0]);
            chdir(path_string.c_str());
            return "";
        }
    } else if (cmd_name == ALIAS) {
        cmd->ensure_has_args(2);
        m_shell_context->add_alias(args[0], args[1]);
        return "";
    } else if (cmd_name == EXPORT) {
        cmd->ensure_has_args(1);
        std::string arg = args[0];
        auto key = arg.substr(0, arg.find('='));
        auto value = arg.substr(arg.find('=') + 1, arg.length());
        setenv(key.c_str(), value.c_str(), true);
        return "";
    } else if (cmd_name == UNEXPORT) {
        cmd->ensure_has_args(1);
        std::string arg = args[0];
        auto key = arg.substr(0, arg.find('='));
        unsetenv(key.c_str());
        return "";
    } else if (cmd_name == ECHO) {
        cmd->ensure_has_args(1);
        std::string arg = args[0];
        if (arg[0] == '$') {
            auto key = arg.substr(1, arg.length());
            std::string value(getenv(key.c_str()));
            return value;
        }
        return arg;
    }
    throw command_not_found_exception(cmd_name.c_str());
}

std::string lsh::cmd::command_handler::handle_extern_cmds(const std::vector<std::shared_ptr<lsh::cmd::command>> &cmds) {
    validate_external_commands(cmds);
    if (cmds.size() == 1) {
        int fd[2];
        auto cmd_to_exec = cmds.front();
        auto cmd_name = cmd_to_exec->get_name();
        if (m_shell_context->alias_exists(cmd_name)) {
            cmd_name = m_shell_context->get_origin_of_alias(cmd_name);
        }
        int child_status;
        const char *c = cmd_name.c_str();
        char *args[cmd_to_exec->arg_count() + 2];
        args[0] = const_cast<char *>(c);
        int i = 1;
        for (auto &arg : cmd_to_exec->get_args()) {
            args[i] = const_cast<char *>(arg.c_str());
            i++;
        }
        args[i] = NULL;
        if (pipe(fd) == -1) {
            return "";
        }
        pid_t p;
        pid_t pid = fork();
        if (pid < 0) {
            std::cerr << "fork failed" << std::endl;
        } else if (pid == 0) {
            if (is_not_sync_command(cmd_to_exec)) {
                close(fd[0]);
                dup2(fd[1], 1);
                dup2(fd[1], 2);
                close(fd[1]);
            }
            execvp(c, args);
            std::cerr << "exec failed" << std::endl;
        } else {
            do {
                p = wait(&child_status);
            } while (p != pid);
            if (is_not_sync_command(cmd_to_exec)) {
                char buffer[2048];
                close(fd[1]);
                while (read(fd[0], buffer, sizeof(buffer)) != 0)
                    ;
                std::string data(buffer);
                return data;
            }
        }
    } else {
        std::string commands;
        std::for_each(cmds.begin(), cmds.end(), [&](const std::shared_ptr<lsh::cmd::command> &cmd) {
            std::string command(cmd->get_name());
            command += " ";
            auto args = cmd->get_args();
            std::for_each(args.begin(), args.end(), [&](const std::string &arg) {
                command = command + " " + arg;
            });
            if (!commands.empty())
                commands = commands + "|" + command;
            else
                commands = command;
        });
        piped_executor executor(cmds);
        return executor.execute();
    }
    return "";
}

bool lsh::cmd::command_handler::is_not_sync_command(std::shared_ptr<lsh::cmd::command> cmd) {
    return cmd->get_name() != "clear";
}

bool lsh::cmd::command_handler::external_cmd_exists(std::shared_ptr<lsh::cmd::command> cmd) {
    return std::find(m_available_commands.begin(), m_available_commands.end(), cmd->get_name()) != m_available_commands.end();
}

void lsh::cmd::command_handler::validate_external_commands(std::vector<std::shared_ptr<lsh::cmd::command>> cmds) {
    for (auto &cmd : cmds) {
        if (!external_cmd_exists(cmd)) {
            throw command_not_found_exception(cmd->get_name().c_str());
        }
    }
}

void lsh::cmd::command_handler::index_path() {
    std::string path(getenv("PATH"));
    const std::string delim = ":";

    size_t pos = 0;
    std::string token;
    while ((pos = path.find(delim)) != std::string::npos) {
        token = path.substr(0, pos);
        for (const auto & entry : std::filesystem::directory_iterator(token))
            m_available_commands.push_back(entry.path().filename().string());
        path.erase(0, pos + delim.length());
    }
}
