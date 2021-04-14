#include <stdio.h>
#include <unistd.h>
#include <vector>

int execute_simple_pipe_example() {
    int pid;
    int p1[2];

    if (pipe(p1) == -1) {
        perror("pipe failed");
        return -1;
    }
    if ((pid = fork()) == -1) {
        perror("fork failed");
        return -1;
    } else if (pid == 0) {
        //child
        dup2(p1[1], 1);// redirect stdin to pipe write
        close(p1[0]);  // we dont need read now
        execlp("ls", "ls", NULL);
        perror("no ls failed");
        _exit(-1);
    }

    if ((pid = fork()) == -1) {
        perror("fork failed");
        return -1;
    } else if (pid == 0) {
        dup2(p1[0], 0);// lets read
        close(p1[1]);
        execlp("sort", "sort", NULL);
        perror("no sort failed");
        _exit(-1);
    }
    return 0;
}

struct command {
    const char **argv;
};

int spawn_proc(int in, int out, struct command *cmd) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        if (in != 0) {
            dup2(in, 0);
            close(in);
        }
        if (out != 1) {
            dup2(out, 1);
            close(out);
        }
        return execvp(cmd->argv[0], (char * const *)cmd->argv);
    }
    return pid;
}

int fork_pipes(int n, struct command *cmd) {
    int i;
    pid_t pid;
    int in, fd[2];
    // first process should get its input from the original file descriptor 0
    in = 0;

    for (i = 0; i < n - 1; ++i) {
        pipe(fd);
        // fd[1] is write of pipe, we carry in from prev iter
        spawn_proc(in, fd[1], cmd + i);

        // no need for the write end of the pipe, the child will write here
        close(fd[1]);

        // keep the read end of the pipe, the next child will read from there
        in = fd[0];
    }
    // last stage set stdin to be the read end of the prev pipe and output to the original file descriptor 1
    if (in != 0) {
        dup2(in, 0);
    }
    // exec the last stage
    return execvp(cmd[i].argv[0], (char * const *)cmd[i].argv);
}

int spawn_command(char **cmd, int input, bool is_first, bool is_last) {
    int fd[2];
    pipe(fd);
    int pid = fork();
    if (pid == 0) {
        if (is_first) {
            dup2(fd[1], STDOUT_FILENO);
        } else if (is_last) {
            dup2(input, STDIN_FILENO);
        } else {
            dup2(input, STDIN_FILENO);
            dup2(fd[1], STDOUT_FILENO);
        }

        if (execvp(cmd[0], cmd) == -1) {
            exit(-1);
        }
    }
    if (input != 0) {
        close(input);
    }
    close(fd[1]);
    if (is_last) {
        close(fd[0]);
    }
    return fd[0];
}

typedef struct {
    char **argv;
}cmd_t;

int main() {
    //    return execute_simple_pipe_example();
    char *ls[] = { "ls", "-l", 0 };
    char *awk[] = { "awk", "{print $1}", 0 };
    char *sort[] = { "sort", 0 };
    char *uniq[] = { "uniq", 0 };
//
    cmd_t cmd [] = { {ls}, {awk}, {sort}, {uniq} };
    int input = 0;
    for (int i = 0; i < 4; i++) {
        input = spawn_command(cmd[i].argv, input, i == 0, i == 3);
    }
//
//    return fork_pipes (4, cmd);
    return 0;
}