#include "builtin.h"

const char* builtinCmdStr[CMD_COUNT] = {
    "unalias",
    "alias",
    "cat",
    "cd",
    "echo",
    "grep*****************",
    "history",
    // "ls",
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
    if (DEBUG)print_seq(seq);
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
        if (DEBUG) printf("builtin.c Executing grep command...\n");
        // 实现grep命令
        break;
    case CMD_HISTORY:
        // 执行history命令
        exec_builtin_history(seq);
        break;
        // case CMD_LS:
        //     printf("builtin.c Executing ls command...\n");
        //     // 实现ls命令
        //     break;
    case CMD_TAB:
        if (DEBUG) printf("builtin.c Executing tab command...\n");
        // 实现tab命令
        break;
    case CMD_TYPE:
        if (DEBUG) printf("builtin.c Executing type command...\n");
        // 实现type命令
        exec_builtin_type(seq);
        break;
    default:
        if (DEBUG) printf("builtin.c Unknown command 需要执行外部命令.\n");
        execute_outer_command(seq);
    }
}

// 执行外部命令
void execute_outer_command(SEQ* seq)
{
    if (DEBUG)printf("builtin.c 109 执行外部命令\n");
    char** new_args = malloc(sizeof(char*) * (seq->args_count + 2)); // 添加命令名cmd_name
    if (new_args == NULL) {
        perror("malloc");
        return;
    }

    // 将 cmd_name 设置为新数组的第一个元素
    new_args[0] = seq->cmd_name;

    // 复制原始 args 到新数组，从 new_args[1] 开始
    for (int i = 0; i < seq->args_count; i++)
        new_args[i + 1] = seq->args[i];

    // 设置新数组的结尾为 NULL
    new_args[seq->args_count + 1] = NULL;

    pid_t pid = fork(); // 创建子进程

    if (pid == -1) {
        // fork 失败
        free(new_args);
        perror("fork");
        exit(EXIT_FAILURE);

    }
    // 子进程
    else if (pid == 0) {
        // 使用 execvp 执行命令，seq->args 应该以 NULL 结尾
        if (execvp(seq->cmd_name, new_args) == -1) {
            // execvp 失败
            perror("execvp");
            exit(EXIT_FAILURE); // 如果 execvp 失败，子进程应该立刻退出
        }
        free(new_args); // 释放 new_args
    }
    // 父进程
    else {
        int status;
        waitpid(pid, &status, 0); // 等待子进程结束
        // 检查子进程的退出状态
        if (WIFEXITED(status)) {
            if (DEBUG)printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
        // 检查子进程是否被信号终止
        else if (WIFSIGNALED(status)) {
            if (DEBUG)printf("Child terminated by signal %d\n", WTERMSIG(status));
        }
        free(new_args);
    }
}