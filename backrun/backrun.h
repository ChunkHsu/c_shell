#ifndef BACKRUN_H
#define BACKRUN_H
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../command.h"
#include "../eval.h"
#include "../global.h"

void exec_backrun(COMMAND* cmd);

#endif // BACKRUN_H