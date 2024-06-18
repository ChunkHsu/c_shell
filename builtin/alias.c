#include "builtin.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 别名列表
Alias* alias_list = NULL;

// 添加别名
void add_alias(const char* alias_name, const char* alias_cmd)
{
    // 检查别名是否是内建命令
    if (is_builtin_command(alias_name)) {
        printf("Error: '%s' is a built-in command and cannot be used as an alias.\n", alias_name);
        return;
    }
    // 遍历别名列表
    Alias* current = alias_list;
    while (current != NULL) {
        if (strcmp(current->alias_name, alias_name) == 0) {
            // 如果找到已存在的别名，更新其命令
            free(current->alias_cmd); // 释放旧命令的内存
            current->alias_cmd = strdup(alias_cmd); // 复制新命令
            return;
        }
        current = current->next;
    }
    // 如果别名不存在，添加新的别名
    Alias* new_alias = (Alias*)malloc(sizeof(Alias));
    new_alias->alias_name = strdup(alias_name);
    new_alias->alias_cmd = strdup(alias_cmd);
    new_alias->next = alias_list;
    alias_list = new_alias;
}

// 根据别名查找对应命令
const char* get_alias_command(const char* alias_name)
{
    Alias* current = alias_list;
    while (current != NULL) {
        if (strcmp(current->alias_name, alias_name) == 0) {
            return current->alias_cmd;
        }
        current = current->next;
    }
    return NULL;
}

/**
 *@brief 打印别名命令
 *
 * @param alias_name
 * @return int
 */
int print_alias_command(const char* alias_name)
{
    Alias* current = alias_list;
    while (current != NULL) {
        if (strcmp(current->alias_name, alias_name) == 0) {
            printf("%s is aliased to `%s`\n", alias_name, current->alias_cmd);
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// 列出所有别名
void list_aliases()
{
    Alias* current = alias_list;
    while (current != NULL) {
        printf("alias: %s -> %s\n", current->alias_name, current->alias_cmd);
        current = current->next;
    }
}

// 释放别名列表的内存（在main程序结束时调用）
void free_alias_list()
{
    Alias* current = alias_list;
    while (current != NULL) {
        Alias* next = current->next;
        free(current->alias_name);
        free(current->alias_cmd);
        free(current);
        current = next;
    }
}

//* 将别名转换为对应的原始命令
/**
 *@brief 获取别名对应的命令
 *
 * @param alias_seq
 * @return SEQ*
 */
SEQ* change_alias_seq_args(SEQ* alias_seq)
{
    // 新建一个 seq 以便修改参数

    // 获取别名对应的命令
    const char* alias_cmd = get_alias_command(alias_seq->cmd_name);
    if (alias_cmd == NULL) {
        return alias_seq; // 别名不存在，返回原来的 seq
    }

    // 创建一个临时字符串以便解析别名命令
    char* temp_cmd = strdup(alias_cmd);
    if (temp_cmd == NULL) {
        return alias_seq; // 内存分配失败，返回原来的 seq
    }

    // 去除单引号和双引号
    int len = strlen(temp_cmd);
    if (temp_cmd[0] == '\'' || temp_cmd[0] == '"') {
        // 如果第一个字符是单引号或双引号，则去除第一个字符
        memmove(temp_cmd, temp_cmd + 1, len); // 移动后面的字符覆盖第一个字符
        temp_cmd[len - 2] = '\0'; // 去除最后一个字符（引号）
    }
    else if (temp_cmd[len - 1] == '\'' || temp_cmd[len - 1] == '"') {
        // 如果最后一个字符是单引号或双引号，则直接去除最后一个字符
        temp_cmd[len - 1] = '\0';
    }

    // 分割别名命令为命令名称和参数
    char* token = strtok(temp_cmd, " ");
    if (token == NULL) {
        free(temp_cmd);
        return alias_seq; // 解析失败，返回原来的 seq
    }

    // 更新命令名称
    free(alias_seq->cmd_name);
    alias_seq->cmd_name = strdup(token);
    if (alias_seq->cmd_name == NULL) {
        free(temp_cmd);
        return alias_seq; // 内存分配失败，返回原来的 seq
    }

    // 计算别名参数的数量并存储
    int alias_arg_count = 0;
    char** alias_args = NULL;
    while ((token = strtok(NULL, " ")) != NULL) {
        alias_args = (char**)realloc(alias_args, (alias_arg_count + 1) * sizeof(char*));
        if (alias_args == NULL) {
            free(temp_cmd);
            return alias_seq; // 内存分配失败，返回原来的 seq
        }
        alias_args[alias_arg_count] = strdup(token);
        if (alias_args[alias_arg_count] == NULL) {
            free(temp_cmd);
            return alias_seq; // 内存分配失败，返回原来的 seq
        }
        alias_arg_count++;
    }

    // 重新分配内存以便将别名的参数添加到原有参数之前
    int total_arg_count = alias_arg_count + alias_seq->args_count;
    char** new_args = (char**)malloc((total_arg_count + 1) * sizeof(char*));
    if (new_args == NULL) {
        free(temp_cmd);
        for (int i = 0; i < alias_arg_count; i++) {
            free(alias_args[i]);
        }
        free(alias_args);
        return alias_seq; // 内存分配失败，返回原来的 seq
    }

    // 复制别名参数和原有参数到新的参数数组
    for (int i = 0; i < alias_arg_count; i++) {
        new_args[i] = alias_args[i];
    }
    for (int i = 0; i < alias_seq->args_count; i++) {
        new_args[alias_arg_count + i] = alias_seq->args[i];
    }
    new_args[total_arg_count] = NULL;

    // 更新 seq 的参数
    free(alias_seq->args);
    alias_seq->args = new_args;
    alias_seq->args_count = total_arg_count;

    // 清理临时数据
    free(temp_cmd);
    free(alias_args); // 这里不用释放 alias_args 里的字符串，因为已经转移到 new_args 了

    return alias_seq;
}

//* alias
void exec_alias(SEQ* seq)
{

    if (seq->args_count == 0) {
        // 如果没有参数，则列出所有别名
        list_aliases();
        return;
    }

    if (seq->args_count > 2) {
        // 如果参数大于2
        printf("args error: too many args\n");
        return;
    }
    // 解析命令参数，args[0] 是别名，args[1] 是别名对应的命令
    const char* alias_name = seq->args[0]; // 别名
    const char* alias_cmd = seq->args[1]; // 别名对应的原始命令

    add_alias(alias_name, alias_cmd);
    printf("alias added: %s -> %s\n", alias_name, alias_cmd);
}

//* 删除别名
void exec_unalias(SEQ* seq)
{
    // 如果没有参数，则提示用法
    if (seq->args_count == 0) {
        printf("unalias: usage: unalias [-a] name [name ...]\n");
        return;
    }

    // 遍历参数
    for (int i = 0;i < seq->args_count;i++) {
        // 如果参数是 -a，则删除所有别名
        if (strcmp(seq->args[i], "-a") == 0) {
            free_alias_list();
            printf("all aliases removed\n");
            return;
        }
        // 否则删除指定别名
        else {
            const char* alias_name = seq->args[0];
            Alias* current = alias_list;
            Alias* prev = NULL;
            while (current != NULL) {
                // 如果找到别名，删除它
                if (strcmp(current->alias_name, alias_name) == 0) {
                    if (prev == NULL) {
                        alias_list = current->next;
                    }
                    else {
                        prev->next = current->next;
                    }
                    free(current->alias_name);
                    free(current->alias_cmd);
                    free(current);
                    printf("alias removed: %s\n", alias_name);
                    return;
                }
                // 否则继续查找
                prev = current;
                current = current->next;
            }
            // 如果找不到别名
            printf("alias not found: %s\n", alias_name);
        }
    }
}

//* 实现alias命令的函数
void exec_builtin_alias(SEQ* seq)
{
    if (strcmp(seq->cmd_name, "unalias") == 0) {
        exec_unalias(seq);
    }
    else {
        exec_alias(seq);
    }
}
