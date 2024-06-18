#ifndef REDIR_H
#define REDIR_H
#include "../command.h"
#include "../eval.h"
#include "../global.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void exec_redir(COMMAND* cmd);
void do_fork(COMMAND* cmd);
void redirect_output(const char* filename);
void redirect_append_output(const char* filename);
void redirect_input(const char* filename);

#endif // REDIR_H