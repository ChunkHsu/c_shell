#include "builtin.h"

/**
 *@brief 执行hsitory命令 -c 清除历史记录 n 显示n条历史记录
 *
 * @param seq
 */
void exec_builtin_history(SEQ* seq)
{
    int n = 0;
    // 如果seq有参数无参数显示全部
    if (seq->args_count == 0) {
        n = history_length;
    }
    // 如果只有一个参数 则显示n条历史记录数量
    else if (seq->args_count == 1) {
        // 如果参数是-c 清除所有历史记录
        if (strcmp(seq->args[0], "-c") == 0) {
            // 清除所有历史记录
            clear_history();
        }
        // 如果参数是数字
        else {
            if (is_number(seq->args[0])) {
                n = atoi(seq->args[0]);
            }
            else {
                fprintf(stderr, "history: %s: numeric argument required\n", seq->args[0]);
                return;
            }
        }
    }
    // 多个参数
    else if (seq->args_count > 1) {
        fprintf(stderr, "history: too many arguments\n");
        return;
    }

    HIST_ENTRY** the_history_list = history_list();
    if (the_history_list) {
        int i;
        int start = history_length - n; // 计算起始位置
        if (start < 0) start = 0; // 确保不会超出历史记录的开始
        for (i = start; i < history_length; ++i) {
            printf("%d: %s\n", i + history_base, the_history_list[i]->line);
        }
    }
}
