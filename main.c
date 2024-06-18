#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "y.tab.h" // 确保包含由 Bison 生成的头文件
#include "global.h"
#include "command.h"
#include "history.h"
#include "eval.h"

#define BUFFER_SIZE 1024

// extern int globalVariable;
// extern COMMAND* global_command;

extern int yyparse(void);
extern FILE* yyin;
extern void yyerror(char* s);

extern void free_alias_list(); // 释放别名列表内存 builtins/alias.c



// gcc -o myprogram main.c parser.c lexer.c -ly -ll
int main()
{
    //* 初始化
    get_prompt(pwd, sizeof(pwd)); // 获取当前工作目录
    char* input; // 用户输入内容
    FILE* file;

    /* 获取用户名和主机名 */
    get_username_and_hostname();

    /* 初始化历史记录 */
    CommandHistory* history_head = NULL;
    CommandHistory* history_tail = NULL;
    CommandHistory* current_history = NULL;

    //* 
    printf("Enter your commands:\n");

    while (1) {
        char prompt[MAX_PATH_SIZE * 2]; // 提示符

        snprintf(prompt, sizeof(prompt), "\x1b[38;5;208m[%s@%s:%s]$\x1b[0m ", username, hostname, pwd);
        input = readline(prompt); // 获取输入
        // 为新字符串分配足够的空间，包括输入、换行符和终止字符
        char* modifiedInput = malloc(strlen(input) + 2);
        if (input && *input) { // 检查输入非空
            add_history(input); // 添加到历史记录

            // 复制输入到新字符串，并添加换行符和终止字符
            strcpy(modifiedInput, input);
            strcat(modifiedInput, "\n");

            yyin = fmemopen(modifiedInput, strlen(modifiedInput), "r"); // 将输入字符串转换为 FILE* 流

            // 调用解析器处理输入
            if (yyparse() == 0) {
                //* 执行命令
                eval_command(global_command);

                printf("main.c 执行命令Parsing success.\n");
            }
            // 解析失败
            else {
                printf("main.c  Parsing failed.\n");
            }

            // 关闭文件
            fclose(yyin); // 关闭流
        }
        // 处理完毕后，释放分配的内存
        free(modifiedInput);
        free(input); // 释放分配的内存
        free_command(global_command); // 释放命令内存
        global_command = NULL; // 重置全局命令指针
    }
    free_alias_list(); // 释放别名列表内存
    free(username);
    free(hostname);

    return 0;
}


// 语法解析错误处理
void yyerror(char* s)
{
    fprintf(stderr, "%s\n", s);
}
