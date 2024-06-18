
#include "backrun.h"

// 后台执行命令
void exec_backrun(COMMAND* cmd)
{
    pid_t pid;
    pid = fork(); // 创建子进程

    if (pid < 0) {
        // fork 失败
        perror("fork");
    }
    // 子进程
    else if (pid == 0) {
        // 子进程执行命令
        cmd->is_background = 0; // 子进程不再是后台进程
        eval_command(cmd);
        _exit(EXIT_FAILURE); // 确保子进程在出错时退出
    }
    // 父进程
    else {
        printf("Running in background, PID: %d\n", pid);
    }
}
