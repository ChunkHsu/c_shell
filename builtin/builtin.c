#include "builtin.h"

const char* builtinCmdStr[CMD_COUNT] = {
    "unalias",
    "alias",
    "cat",
    "cd",
    "echo",
    "grep",
    "history",
    "ls",
    "tab",
    "type",
    // 其他命令的字符串表示...
};

/**
 *@brief 是否是内部
 *
 * @param cmd_name
 * @return int
 */
int is_builtin_command(const char* cmd_name)
{
    for (int i = 0; i < CMD_COUNT; ++i) {
        if (strcmp(builtinCmdStr[i], cmd_name) == 0) {
            return 1;
        }
    }
    return 0;
}


/********************************************/
/*              执行内部命令                 */
/*                                          */
/********************************************/

BuiltinCmd getBuiltinCmd(const char* input)
{
    for (int i = 0; i < CMD_COUNT; i++) {
        if (strcmp(input, builtinCmdStr[i]) == 0) {
            return (BuiltinCmd)i;
        }
    }
    return CMD_COUNT; // 如果不匹配任何内部命令，则是外部命令
}

// 执行命令
void exec_cmd(SEQ* seq)
{
    if (!seq || !seq->cmd_name) {
        // 如果 seq 为空或命令名为空，则返回
        return;
    }
    // 将别名转换为对应的原始命令
    seq = change_alias_seq_args(seq);
    print_seq(seq);
    // 获取命令序号
    BuiltinCmd cmd = getBuiltinCmd(seq->cmd_name);

    switch (cmd) {
    case CMD_UNALIAS:
    case CMD_ALIAS:
        // 执行alias 与 unalias 命令
        exec_builtin_alias(seq);
        break;
    case CMD_CAT:
        // 执行cat命令
        exec_builtin_cat(seq);
        break;
    case CMD_CD:
        // 执行cd命令
        exec_builtin_cd(seq);
        break;
    case CMD_ECHO:
        // 执行echo命令
        exec_builtin_echo(seq);
        break;
    case CMD_GREP:
        printf("builtin.c Executing grep command...\n");
        // 实现grep命令
        break;
    case CMD_HISTORY:
        // 执行history命令
        exec_builtin_history(seq);
        break;
    case CMD_LS:
        printf("builtin.c Executing ls command...\n");
        // 实现ls命令
        break;
    case CMD_TAB:
        printf("builtin.c Executing tab command...\n");
        // 实现tab命令
        break;
    case CMD_TYPE:
        printf("builtin.c Executing type command...\n");
        // 实现type命令
        break;
    default:
        printf("builtin.c Unknown command 需要执行外部命令.\n");
        execute_outer_command(seq);
    }
}

// 执行外部命令
void execute_outer_command(SEQ* seq)
{
    printf("builtin.c execute_outer_command\n");
    pid_t pid = fork(); // 创建子进程

    if (pid == -1) {
        // fork 失败
        perror("fork");
        exit(EXIT_FAILURE);
    }
    // 子进程
    else if (pid == 0) {
        // 使用 execvp 执行命令，seq->args 应该以 NULL 结尾
        if (execvp(seq->cmd_name, seq->args) == -1) {
            // execvp 失败
            perror("execvp");
            exit(EXIT_FAILURE); // 如果 execvp 失败，子进程应该立刻退出
        }
    }
    // 父进程
    else {
        int status;
        waitpid(pid, &status, 0); // 等待子进程结束
        if (WIFEXITED(status)) {
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status)) {
            printf("Child terminated by signal %d\n", WTERMSIG(status));
        }
    }
}