#include "builtin.h"
#include <stdio.h>
#include <unistd.h>

void builtin_cd(SEQ* seq)
{
    if (seq->args_count < 2) {
        fprintf(stderr, "cd: missing argument\n");
        return;
    }
    if (chdir(seq->args[1]) != 0) {
        perror("cd failed");
    }
}
