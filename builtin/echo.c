#include "builtin.h"
#include <stdio.h>

void builtin_echo(SEQ* seq)
{
    for (int i = 1; i < seq->args_count; i++) {
        printf("%s ", seq->args[i]);
    }
    printf("\n");
}
