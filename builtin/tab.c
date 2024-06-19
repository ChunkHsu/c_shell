#include "builtin.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>


/**
 *@brief tab补全
 *
 * @param seq
 */
void exec_builtin_tab_completion(SEQ* seq)
{
    if (seq->args_count > 0) {
        tab_completion(seq->args[0]); // 假设第一个参数是需要补全的文本
    }
}

/**
 *@brief 根据输入补全命令或路径
 *
 * @param input
 */
void tab_completion(const char* input)
{
    char dirPath[1024]; // 保存当前目录路径
    char prefix[256]; // 保存路径前缀
    // 命令补全
    if (strchr(input, '/') == NULL) { // 没有路径分隔符，假设补全命令
        char* path = getenv("PATH"); // 获取PATH环境变量
        if (path == NULL) {
            return;
        }
        char* pathCopy = strdup(path); // 复制PATH环境变量
        if (pathCopy == NULL) {
            return;
        }

        char* dirPath = strtok(pathCopy, ":"); // 分割路径
        while (dirPath != NULL) { // 遍历每个目录
            list_commands_in_directory(dirPath, input); // 列出目录中以指定前缀开头的命令
            dirPath = strtok(NULL, ":"); // 继续下一个目录
        }
        free(pathCopy);
    }
    else { // 路径补全
        const char* lastSlash = strrchr(input, '/'); // 找到最后一个斜杠
        if (lastSlash != NULL) { // 如果有斜杠，分割路径和前缀
            size_t dirLen = lastSlash - input + 1; // 包括最后的斜杠
            strncpy(dirPath, input, dirLen); // 复制路径
            dirPath[dirLen] = '\0'; // 添加字符串结束符
            strcpy(prefix, lastSlash + 1); // 复制前缀
        }
        else {
            strcpy(dirPath, "./"); // 没有斜杠，假设当前目录
            strcpy(prefix, input); // 整个输入作为前缀
        }

        DIR* dir = opendir(dirPath); // 打开目录
        if (dir == NULL) { // 打开失败
            perror("opendir");
            return;
        }

        struct dirent* entry; // 目录项
        while ((entry = readdir(dir)) != NULL) { // 读取目录项
            if (strncmp(prefix, entry->d_name, strlen(prefix)) == 0) { // 如果前缀匹配，打印出完整的路径
                printf("%s%s\n", dirPath, entry->d_name);
            }
        }

        closedir(dir);
    }
}


/**
 *@brief 列出目录中以指定前缀开头的命令
 *
 * @param dirPath 目录路径
 * @param prefix 前缀
 */
void list_commands_in_directory(const char* dirPath, const char* prefix)
{
    DIR* dir = opendir(dirPath); // 打开目录
    if (dir == NULL) {
        return;
    }

    struct dirent* entry; // 目录项
    while ((entry = readdir(dir)) != NULL) { // 读取目录项
        if (strncmp(entry->d_name, prefix, strlen(prefix)) == 0) { // 如果前缀匹配，打印出来
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
}