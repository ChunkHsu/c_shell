#ifndef COMMAND_H
#define COMMAND_H
#include <stdlib.h>

/* command 类型 */
#define SimpType        0x0000 // 普通命令
#define RedirType       0x0001 // 重定向命令  >
#define PipeType        0x0002 // 管道命令    |
#define BackType        0x0004 // 后台命令    &

struct seq
{
    char* cmd_name; // 命令名称
    char** args; // 参数，使用NULL终止的字符指针数组
    int args_count; // 参数数量
};
typedef struct seq SEQ;

struct command
{
    SEQ* seqs; // 命令序列，动态数组
    char* connect; // 命令连接符
    int seqs_count; // 序列数量
    int type;
    int is_background; // 是否是后台命令
};
typedef struct command COMMAND;

SEQ* init_seq(char* cmd_name);
SEQ* add_seq_args(SEQ* seq, char* arg);
void free_seq(SEQ* seq);
COMMAND* init_command();
COMMAND* add_command(COMMAND* command, SEQ* seq, char* connect);
COMMAND* set_command_type(COMMAND* command, int type);
void free_command(COMMAND* cmd);
void print_command(COMMAND* global_command);

#endif
