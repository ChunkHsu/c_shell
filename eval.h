#ifndef EVAL_H
#define EVAL_H

#include "command.h"

void eval_command(COMMAND* cmd);
void execute_simple_command(COMMAND* cmd);
void execute_redirection_command(COMMAND* cmd);
void execute_pipe_command(COMMAND* cmd);
void execute_background_command(COMMAND* cmd);

// 内建命令
int is_builtin_command(SEQ* seq);
void execute_builtin_command(SEQ* seq);


#endif // EVAL_H