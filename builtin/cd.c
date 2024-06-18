#include "builtin.h"

void exec_builtin_cd(SEQ* seq)
{
    // 如果 cd 命令没有参数，则跳转到用户的 home 目录
    if (seq->args_count == 0) {
        // 获取用户的 home 目录路径
        const char* home_dir;
        if ((home_dir = getenv("HOME")) == NULL) {
            // 如果获取失败，则尝试获取当前用户的 home 目录
            struct passwd* pw = getpwuid(getuid());
            if (pw != NULL) {
                home_dir = pw->pw_dir;
            }
            else {
                fprintf(stderr, "cd: 获取用户 home 目录失败\n");
                return;
            }
        }

        // 执行 cd 命令到用户的 home 目录
        if (chdir(home_dir) == -1) {
            perror("cd"); // 打印错误信息，如果出错
        }
    }
    // 如果 cd 命令有一个参数，则跳转到目标路径
    else if (seq->args_count == 1) {
        // 获取目标路径参数
        char* target_path = seq->args[0];

        // 执行 cd 命令
        if (chdir(target_path) == -1) {
            perror("cd"); // 打印错误信息，如果出错
        }
    }
    else {
        printf("cd: too many arguments\n");
        return;
    }
    // 更新当前工作目录
    get_prompt(pwd, sizeof(pwd)); // 获取当前工作目录
}