#include "builtin.h"

void exec_builtin_cat(SEQ* seq)
{
    bool option_n = false; // 显示所有行的行号
    bool option_b = false; // 只为非空行显示行号
    bool option_s = false; // 压缩连续空行
    int line_number = 1;   // 行号计数器
    bool was_empty_line = false; // 上一行是否为空行标志

    // 解析选项
    for (int i = 0; i < seq->args_count; ++i) {
        if (strcmp(seq->args[i], "-n") == 0) {
            option_n = true;
        }
        else if (strcmp(seq->args[i], "-b") == 0) {
            option_b = true;
        }
        else if (strcmp(seq->args[i], "-s") == 0) {
            option_s = true;
        }
    }

    // 处理文件名参数
    if (seq->args_count > 0) {
        // 遍历所有参数
        for (int i = 0; i < seq->args_count; ++i) {
            // 跳过选项参数
            if (seq->args[i][0] == '-') {
                continue;
            }

            FILE* file = fopen(seq->args[i], "r");
            if (file == NULL) {
                perror("fopen");
                continue;
            }

            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                bool is_empty_line = (buffer[0] == '\n');

                // 处理 -s 选项
                if (option_s && is_empty_line && was_empty_line) {
                    continue;
                }
                was_empty_line = is_empty_line;

                // 处理 -n 和 -b 选项
                if (option_n || (option_b && !is_empty_line)) {
                    printf("%6d\t", line_number++);
                }

                printf("%s", buffer);
            }
            fclose(file);
            was_empty_line = false; // 重置空行标志，以便处理下一个文件
        }
    }
    // 没有提供文件名参数，从标准输入读取
    else {
        printf("Please input text, press Ctrl+D to finish:\n");
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            printf("%s", buffer);
        }
    }

}

