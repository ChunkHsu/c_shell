#include "builtin.h"
#include <stdio.h>

// alias ll='ls -l'
void exec_builtin_type(SEQ* seq)
{
    // 实现 type 命令
    if (DEBUG) printf("type command executed\n");
    // 如果 seq->args_count 为空，则返回
    if (seq->args_count == 0) {
        if (DEBUG) printf("type: missing argument\n");
        return;
    }

    for (int i = 0;i < seq->args_count;i++) {
        if (seq->args[i] == NULL) {
            continue;
        }
        const char* alias_cmd = get_alias_command(seq->args[i]);
        // 检查命令是否是别名
        if (alias_cmd != NULL) {
            printf("%s is aliased to %s\n", seq->args[i], alias_cmd);
        }
        // 检查命令是内置命令
        else if (is_builtin_command(seq->args[i])) {
            printf("%s is a shell builtin\n", seq->args[i]);
        }
        // 检查命令是否是外部命令
        else if (is_external_command(seq->args[i])) {
            // printf("%s is an external command\n", seq->args[i]);
        }
        else {
            printf("%s not found\n", seq->args[i]);
        }
    }

}

// 检查命令是否在指定目录中
int check_command_in_dir(const char* dir, const char* cmd_name, char* fullpath, size_t fullpath_size)
{
    snprintf(fullpath, fullpath_size, "%s/%s", dir, cmd_name);
    return access(fullpath, X_OK) == 0;
}

// 检查命令是否为外部命令并显示其位置
int is_external_command(const char* cmd_name)
{
    char fullpath[1024];

    // 检查命令是否在当前目录中
    if (check_command_in_dir(".", cmd_name, fullpath, sizeof(fullpath))) {
        printf("%s is in the current directory (%s)\n", cmd_name, fullpath);
        return 1;
    }

    // 尝试在 PATH 环境变量指定的目录中查找命令
    char* path = getenv("PATH");
    if (!path) return 0;  // 如果 PATH 未设置，直接返回

    char* path_copy = strdup(path);  // 复制环境变量字符串，因为 strtok 会修改原字符串
    if (!path_copy) return 0; // 内存分配失败

    int found = 0;
    char* p = strtok(path_copy, ":");
    while (p != NULL) {
        if (check_command_in_dir(p, cmd_name, fullpath, sizeof(fullpath))) {
            // 命令存在并可执行
            printf("%s is hashed %s (%s)\n", cmd_name, p, fullpath);
            found = 1;
            break;
        }
        p = strtok(NULL, ":");
    }

    free(path_copy);
    return found;
}


