#ifndef EVAL_H
#define EVAL_H

#include "command.h"
#include "global.h"
#include "builtin/builtin.h"
#include "redir/redir.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>


void eval_command(COMMAND* cmd);
void execute_simple_command(COMMAND* cmd);
void execute_redirection_command(COMMAND* cmd);
void execute_pipe_command(COMMAND* cmd);
void execute_background_command(COMMAND* cmd);


#endif // EVAL_H