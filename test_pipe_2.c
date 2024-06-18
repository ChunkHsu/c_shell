#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define oops(m,x) {perror(m);exit(x);}

int main(int ac, char** av)
{
    int thepipe[2], newfd, pid;
    if (ac != 3) {
        fprintf(stderr, "usage: pipe cmd1 cmd2\n");
        exit(1);
    }
    if (pipe(thepipe) == -1) {
        oops("Cannot get a pipe", 1);
    }
    if ((pid = fork()) == -1) {
        oops("Cannot fork", 2);
    }

    // child cmd1
    if (pid == 0) {
        close(thepipe[0]); //关闭读段
        if (dup2(thepipe[1], 1) == -1) { // 标准输出重定向到管道的写端
            oops("could not redirect stdout", 4);
        }
        close(thepipe[1]); //关闭写段
        execlp(av[1], av[1], NULL); // 运行命令1
        oops(av[1], 5);
    }

    // parent cmd2
    close(thepipe[1]); //关闭写段
    if (dup2(thepipe[0], 0) == -1) { // 将标准输入重定向到读端
        oops("could not redirect stdin", 3);
    }
    close(thepipe[0]); //关闭读段
    execlp(av[2], av[2], NULL); // 运行命令2
    oops(av[2], 4);
}