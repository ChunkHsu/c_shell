#ifndef BUILTIN_H
#define BUILTIN_H

#include "../command.h"  // 修正包含路径
#include "../eval.h"
#include "../global.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h> // 用于 chdir 函数
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <pwd.h>    // 用于获取用户信息
#include <string.h>
#include <stdlib.h>

#define bool _Bool


typedef enum
{
    CMD_UNALIAS,
    CMD_ALIAS,
    CMD_CAT,
    CMD_CD,
    CMD_ECHO,
    CMD_GREP,
    CMD_HISTORY,
    CMD_LS,
    CMD_TAB,
    CMD_TYPE,
    // 其他命令...
    CMD_COUNT     // 枚举值的数量，始终在最后
} BuiltinCmd;

extern const char* builtinCmdStr[CMD_COUNT];


// 内建命令函数声明
void exec_builtin_alias(SEQ* seq);
void exec_builtin_cat(SEQ* seq);
void exec_builtin_cd(SEQ* seq);
void exec_builtin_echo(SEQ* seq);
void exec_builtin_grep(SEQ* seq);
void exec_builtin_history(SEQ* seq);
void exec_builtin_ls(SEQ* seq);
void exec_builtin_tab_completion(SEQ* seq);
void exec_builtin_type(SEQ* seq);

// 内置命令
int is_builtin_command(const char* cmd_name);
SEQ* change_alias_seq_args(SEQ* alias_seq);
BuiltinCmd getBuiltinCmd(const char* input);
void exec_cmd(SEQ* seq);
void execute_outer_command(SEQ* seq);


#endif // BUILTIN_H
