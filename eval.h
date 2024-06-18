#ifndef EVAL_H
#define EVAL_H

#include "command.h"

void eval_command(COMMAND* cmd);
void execute_simple_command(COMMAND* cmd);
void execute_redirection_command(COMMAND* cmd);
void execute_pipe_command(COMMAND* cmd);
void execute_background_command(COMMAND* cmd);


#endif // EVAL_H