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
    // CMD_LS,
    CMD_TAB,
    CMD_TYPE,
    // 其他命令...
    CMD_COUNT     // 枚举值的数量，始终在最后
} BuiltinCmd;


// 用链表存储别名
typedef struct Alias
{
    char* alias_name; // 别名
    char* alias_cmd; // 别名对应的原始命令
    struct Alias* next; // 下一个别名
} Alias;

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
const char* get_alias_command(const char* alias_name);
int check_command_in_dir(const char* dir, const char* cmd_name, char* fullpath, size_t fullpath_size);
int is_external_command(const char* cmd_name);
char* get_cmd_from_PATH_and_builtin();

#endif // BUILTIN_H
