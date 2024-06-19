#include "pipe.h"

void oops(const char* msg, int err)
{
    // 打开或创建 .error 文件，追加写入
    int fd = open(".error", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("open");
        exit(err);
    }
    // 备份 stderr
    int stderr_backup = dup(STDERR_FILENO);
    if (stderr_backup == -1) {
        perror("dup");
        exit(err);
    }
    // 将 stderr 重定向到 .error 文件
    if (dup2(fd, STDERR_FILENO) == -1) {
        perror("dup2");
        exit(err);
    }
    // 写入错误信息
    perror(msg);
    // 恢复 stderr
    if (dup2(stderr_backup, STDERR_FILENO) == -1) {
        perror("dup2");
        exit(err);
    }
    close(fd);
    close(stderr_backup);
    exit(err);
}

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
        oops("exec_cmd执行单个命令0出错", 5);
    }

    if ((pid2 = fork()) == -1) {
        oops("Cannot fork", 2);
    }

    // command | right
    if (pid2 == 0) { // 子进程2
        close(thepipe[1]); // 关闭写端
        if (dup2(thepipe[0], 0) == -1) { // 如果将标准输入重定向到读端出错
            oops("could not redirect stdin", 3);
        }
        close(thepipe[0]); // 关闭读端
        exec_cmd(&(cmd->seqs[1])); // 运行命令2
        oops("exec_cmd执行单个命令1出错", 4);
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
        if (DEBUG) printf("Child 1 exited with status %d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status)) {
        if (DEBUG) printf("Child 1 was killed by signal %d\n", WTERMSIG(status));
    }
    else {
        if (DEBUG) printf("Child 1 exited abnormally\n");
    }

    if (waitpid(pid2, &status, 0) == -1) {
        oops("waitpid failed", 6);
    }
    if (WIFEXITED(status)) {
        if (DEBUG) printf("Child 2 exited with status %d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status)) {
        if (DEBUG) printf("Child 2 was killed by signal %d\n", WTERMSIG(status));
    }
    else {
        if (DEBUG) printf("Child 2 exited abnormally\n");
    }
}

