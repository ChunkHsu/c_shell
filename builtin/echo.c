#include "builtin.h"


// 替换字符串中的环境变量
char* replace_env_vars(const char* str)
{
    // 估计替换后的字符串长度（假设环境变量不会超过100个字符）
    size_t estimated_len = strlen(str) + 1;
    char* result = malloc(estimated_len);
    if (result == NULL) {
        return NULL; // 内存分配失败
    }
    result[0] = '\0';

    const char* current = str;
    while (*current != '\0') {
        if (*current == '$') {
            // 找到一个环境变量
            const char* end = current + 1;
            while ((*end >= 'a' && *end <= 'z') || (*end >= 'A' && *end <= 'Z') || (*end >= '0' && *end <= '9') || *end == '_') {
                ++end;
            }

            // 提取环境变量名
            size_t var_name_len = end - (current + 1);
            char var_name[var_name_len + 1];
            strncpy(var_name, current + 1, var_name_len);
            var_name[var_name_len] = '\0';

            // 获取环境变量值
            const char* var_value = getenv(var_name);
            if (!var_value) var_value = ""; // 如果环境变量不存在，使用空字符串

            // 确保result有足够的空间
            size_t new_len = strlen(result) + strlen(var_value) + strlen(end) + 1;
            if (new_len > estimated_len) {
                estimated_len = new_len;
                result = realloc(result, estimated_len);
                if (result == NULL) {
                    return NULL; // 内存重新分配失败
                }
            }

            // 添加环境变量值
            strcat(result, var_value);
            current = end;
        }
        else {
            // 添加当前字符到结果字符串
            size_t len = strlen(result);
            if (len + 2 > estimated_len) {
                estimated_len = len + 2;
                result = realloc(result, estimated_len);
                if (result == NULL) {
                    return NULL; // 内存重新分配失败
                }
            }
            result[len] = *current;
            result[len + 1] = '\0';
            ++current;
        }
    }

    return result;
}

void exec_builtin_echo(SEQ* seq)
{
    // 检查是否有参数
    if (seq->args_count < 1) {
        // 如果没有参数，只输出换行
        printf("\n");
        return;
    }

    // 遍历参数
    for (int i = 0; i < seq->args_count; i++) {
        char* arg = seq->args[i];
        int len = strlen(arg);

        // 检查是否是带引号的字符串
        if (arg[0] == '"' && arg[len - 1] == '"') {
            // 创建一个新字符串去除引号
            char* no_quotes = strndup(arg + 1, len - 2);

            // 替换字符串中的环境变量
            char* replaced = replace_env_vars(no_quotes);
            if (replaced) {
                printf("%s", replaced);
                free(replaced);
            }

            free(no_quotes);
        }

        else if (arg[0] == '\'' && arg[len - 1] == '\'') {
            // 去除首尾引号
            arg[len - 1] = '\0';
            arg++;
            printf("%s", arg);
        }
        else {
            // 创建一个新字符
            char* com = strndup(arg, len - 1);
            // 替换字符串中的环境变量
            char* rep = replace_env_vars(com);
            if (rep) {
                printf("%s", rep);
                free(rep);
            }

            free(com);
        }
    }
    printf("\n");
}
