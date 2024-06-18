#ifndef HISTORY_H
#define HISTORY_H

#include "command.h"

typedef struct command_history
{
    COMMAND* cmd; // 存储命令的链表
    struct command_history* prev_cmd; // 上个命令
    struct command_history* next_cmd; // 下个命令
} CommandHistory;

void add_command_to_history(CommandHistory** head, CommandHistory** tail, COMMAND* command);
void print_n_command_history(CommandHistory* tail, int n);
CommandHistory* prev_command_history(CommandHistory* current, CommandHistory* history_head);
CommandHistory* next_command_history(CommandHistory* current, CommandHistory* history_head);
void free_command_history(CommandHistory* history);

#endif // HISTORY_H
