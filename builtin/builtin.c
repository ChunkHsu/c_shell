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
        printf("builtin.c Executing alias and unalias command...\n");
        // 实现alias 与 unalias 命令
        exec_builtin_alias(seq);
        break;
    case CMD_CAT:
        printf("builtin.c Executing cat command...\n");
        // 实现cat命令
        break;
    case CMD_CD:
        printf("builtin.c Executing cd command...\n");
        // 实现cd命令
        exec_builtin_cd(seq);
        break;
    case CMD_ECHO:
        printf("builtin.c Executing echo command...\n");
        // 实现echo命令
        exec_builtin_echo(seq);
        break;
    case CMD_GREP:
        printf("builtin.c Executing grep command...\n");
        // 实现grep命令
        break;
    case CMD_HISTORY:
        printf("builtin.c Executing history command...\n");
        // 实现history命令
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
}