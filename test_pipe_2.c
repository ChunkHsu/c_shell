#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
    int len, i, apipe[2];
    char buf[BUFSIZ];
    if (pipe(apipe) == -1) {
        perror("pipe error");
        exit(1);
    }
    printf("get a pipe! file descriptors are %d and %d\n", apipe[0], apipe[1]);

    while (fgets(buf, BUFSIZ, stdin)) { // 从标准输入读取
        len = strlen(buf); // 计算字符串长度
        // 写入管道
        if (write(apipe[1], buf, len) != len) {
            perror("writing to pipe");
            break;
        }

        for (i = 0; i < len; i++) { // 将缓冲区清零
            buf[i] = 'X';
        }

        // 从管道读取
        len = read(apipe[0], buf, BUFSIZ);

        if (len == -1) {
            perror("reading from pipe");
            break;
        }

        if (write(1, buf, len) != len) { // 标准输出
            perror("writing to stdout");
            break;
        }
    }
}