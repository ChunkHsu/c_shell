#include "pipe.h"

/**
 *@brief 执行单个命令
 *
 * @param seq
 */
 // void exec_seq(SEQ* seq)
 // {
 //     // 执行单个普通命令。
 //     exec_cmd(seq);
 //     // 如果 execvp 返回，说明发生了错误
 //     perror("exec_cmd执行单个命令"); // 打印错误信息
 //     exit(EXIT_FAILURE);
 // }

 /**
  *@brief 执行管道命令
  *
  * @param cmd
  */
void exec_pipe(COMMAND* cmd)
{
    // 创建管道 对于 N 个命令，需要创建 N-1 个管道来连接
    int i, fd[2 * (cmd->seqs_count - 1)]; // fd[0] 是第一个管道的读端，fd[1] 是第一个管道的写端 fd[2] 是第二个管道的读端，fd[3] 是第二个管道的写端，以此类推
    for (i = 0; i < cmd->seqs_count - 1; ++i) {
        // pipe[0]读取 pipe[1]写入 <unistd.h>
        if (pipe(fd + i * 2) < 0) { // 创建管道
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // 为每个命令序列创建进程
    for (i = 0; i < cmd->seqs_count; ++i) { // 创建子进程
        pid_t pid = fork();
        // 子进程 设置管道
        if (pid == 0) {
            if (i > 0) {
                dup2(fd[(i - 1) * 2], 0); // 将标准输入重定向到前一个管道的读端
            }
            if (i < cmd->seqs_count - 1) {
                dup2(fd[i * 2 + 1], 1); // 将标准输出重定向到当前管道的写端
            }

            // 关闭所有管道文件描述符
            for (int j = 0; j < 2 * (cmd->seqs_count - 1); ++j) {
                close(fd[j]);
            }
            if (DEBUG) { printf("执行管道命令：");print_seq(&(cmd->seqs[i])); }
            printf("执行管道命令：");print_seq(&(cmd->seqs[i]));
            exec_cmd(&(cmd->seqs[i]));
        }
        else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    // 在父进程中关闭所有管道文件描述符
    for (i = 0; i < 2 * (cmd->seqs_count - 1); ++i) {
        close(fd[i]);
    }

    // 等待所有子进程结束
    for (i = 0; i < cmd->seqs_count; ++i) {
        wait(NULL);
    }
}