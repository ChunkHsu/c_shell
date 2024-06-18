#include "eval.h"
#include "global.h"
#include "builtin/builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

// echo 打印数据以及$(PATH)

// cat 

// alias 别名

// 后台执行显示PID

// 

/**
 *@brief 检查命令类型并执行相应操作
 *
 * @param cmd
 */
void eval_command(COMMAND* cmd)
{
    // 如果是管道命令
    if (cmd->type & PipeType) {
        execute_pipe_command(cmd);
    }
    // 如果是重定向命令
    else if (cmd->type & RedirType) {
        execute_redirection_command(cmd);
    }
    // 如果是后台命令
    else if (cmd->type & BackType) {
        execute_background_command(cmd);
    }
    // 普通命令
    else {
        execute_simple_command(cmd);
    }
}

// 执行普通命令
void execute_simple_command(COMMAND* cmd)
{
    // 遍历每个命令序列,判断是否是内建命令
    for (int i = 0; i < cmd->seqs_count; i++) {
        exec_cmd(&(cmd->seqs[i]));
    }
    printf("eval.c execute_simple_command\n");
    // print_command(global_command);
}

void execute_redirection_command(COMMAND* cmd)
{
    printf("eval.c execute_redirection_command\n");
    print_command(global_command);
}

void execute_pipe_command(COMMAND* cmd)
{
    printf("eval.c execute_pipe_command\n");
    print_command(global_command);


}

void execute_background_command(COMMAND* cmd)
{
    printf("eval.c execute_background_command\n");
    print_command(global_command);

}
