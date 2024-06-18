#include "builtin.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 假设用链表存储别名
typedef struct Alias
{
    char* alias_name; // 别名
    char* alias_cmd; // 别名对应的原始命令
    struct Alias* next; // 下一个别名
} Alias;

// 别名列表
Alias* alias_list = NULL;

// 添加别名
void add_alias(const char* alias_name, const char* alias_cmd)
{
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
