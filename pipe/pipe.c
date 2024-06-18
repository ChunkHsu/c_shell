#include "pipe.h"

// 执行管道命令
void exec_pipe(COMMAND* cmd)
{
    int thepipe[2], pid1, pid2;

    if (pipe(thepipe) == -1) {
        oops("Cannot get a pipe", 1);
    }
    if ((pid1 = fork()) == -1) {
        oops("Cannot fork", 2);
    }

    // left | command
    if (pid1 == 0) { // 子进程1
        close(thepipe[0]); // 关闭读端
        if (dup2(thepipe[1], 1) == -1) { // 标准输出重定向到管道的写端
            oops("could not redirect stdout", 4);
        }
        close(thepipe[1]); // 关闭写端
        exec_cmd(&(cmd->seqs[0]));
        // oops("exec_cmd执行单个命令0出错", 5);
    }

    if ((pid2 = fork()) == -1) {
        oops("Cannot fork", 2);
    }

    // command | right
    if (pid2 == 0) { // 子进程2
        close(thepipe[1]); // 关闭写端
        if (dup2(thepipe[0], 0) == -1) { // 将标准输入重定向到读端
            oops("could not redirect stdin", 3);
        }
        close(thepipe[0]); // 关闭读端
        exec_cmd(&(cmd->seqs[1])); // 运行命令2
        // oops("exec_cmd执行单个命令1出错", 4);
    }

    // 关闭父进程中的管道端口
    close(thepipe[0]);
    close(thepipe[1]);

    // 父进程等待所有子进程完毕
    int status;
    if (waitpid(pid1, &status, 0) == -1) {
        oops("waitpid failed", 6);
    }
    if (WIFEXITED(status)) {
        printf("Child 1 exited with status %d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status)) {
        printf("Child 1 was killed by signal %d\n", WTERMSIG(status));
    }
    else {
        printf("Child 1 exited abnormally\n");
    }

    if (waitpid(pid2, &status, 0) == -1) {
        oops("waitpid failed", 6);
    }
    if (WIFEXITED(status)) {
        printf("Child 2 exited with status %d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status)) {
        printf("Child 2 was killed by signal %d\n", WTERMSIG(status));
    }
    else {
        printf("Child 2 exited abnormally\n");
    }
}


/**
 *@brief 执行管道命令
 *
 * @param cmd
 */
 // void exec_pipe_n(COMMAND* cmd)
 // {
 //     // 创建管道 对于 N 个命令，需要创建 N-1 个管道来连接
 //     int i, fd[2 * (cmd->seqs_count - 1)]; // fd[0] 是第一个管道的读端，fd[1] 是第一个管道的写端 fd[2] 是第二个管道的读端，fd[3] 是第二个管道的写端，以此类推
 //     for (i = 0; i < cmd->seqs_count - 1; ++i) {
 //         // pipe[0]读取 pipe[1]写入 <unistd.h>
 //         if (pipe(fd + i * 2) < 0) { // 创建管道
 //             perror("pipe");
 //             exit(EXIT_FAILURE);
 //         }
 //     }

 //     // 为每个命令序列创建进程
 //     for (i = 0; i < cmd->seqs_count; ++i) { // 创建子进程
 //         pid_t pid = fork();
 //         // 子进程 设置管道
 //         if (pid == 0) {
 //             if (i > 0) {
 //                 dup2(fd[(i - 1) * 2], 0); // 将标准输入重定向到前一个管道的读端
 //             }
 //             if (i < cmd->seqs_count - 1) {
 //                 dup2(fd[i * 2 + 1], 1); // 将标准输出重定向到当前管道的写端
 //             }

 //             // 关闭所有管道文件描述符
 //             for (int j = 0; j < 2 * (cmd->seqs_count - 1); ++j) {
 //                 close(fd[j]);
 //             }
 //             if (DEBUG) { printf("执行管道命令：");print_seq(&(cmd->seqs[i])); }
 //             printf("执行管道命令：");print_seq(&(cmd->seqs[i]));
 //             exec_cmd(&(cmd->seqs[i]));
 //         }
 //         else if (pid < 0) {
 //             perror("fork");
 //             exit(EXIT_FAILURE);
 //         }
 //     }

 //     // 在父进程中关闭所有管道文件描述符
 //     for (i = 0; i < 2 * (cmd->seqs_count - 1); ++i) {
 //         close(fd[i]);
 //     }

 //     // 等待所有子进程结束
 //     for (i = 0; i < cmd->seqs_count; ++i) {
 //         wait(NULL);
 //     }
 // }