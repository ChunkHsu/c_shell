#ifndef PIPE_H
#define PIPE_H

#include "../command.h"
#include "../eval.h"
#include "../global.h"
#include "../builtin/builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

void exec_pipe(COMMAND* cmd);

#endif // PIPE_H