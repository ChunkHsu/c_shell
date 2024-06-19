#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include "input.h"
#include "history.h"

struct termios orig_termios; // 原始终端模式，用于保存原始终端设置，处理特殊上下键盘输入

/**
 *@brief 关闭原始终端模式
 *
 */
void disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

/**
 *@brief 开启原始终端 keypress 模式
 *
 */
void enable_raw_mode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/**
 *@brief 获取键盘输入
 *
 * @param buffer 输入缓冲区
 * @param size 缓冲区大小
 * @param current_history 当前历史记录
 * @param history_tail 历史记录尾
 * @return int
 */
int get_input(char* buffer, int size, CommandHistory** current_history,
    CommandHistory* history_head, CommandHistory* history_tail)
{
    int index = 0; // 缓冲区索引
    char ch; // 读取的字符

    while (index < size - 1) { // 留一个位置给 NULL
        ch = getchar();
        if (ch == '\n') {  // 如果为回车键，结束输入
            buffer[index] = '\0';
            printf("\n");
            break;
        }
        else if (ch == 27) { // 如果为箭头键
            ch = getchar();
            if (ch == 91) {
                ch = getchar();
                if (ch == 'A') { // 上箭头
                    // 获取上一条历史记录
                    CommandHistory* prev = prev_command_history(*current_history, history_tail);
                    if (prev) { // 如果有上一条历史记录
                        *current_history = prev;
                        printf("\r\x1b[2K"); // 清除当前行
                        printf("\x1b[38;5;208m[%s@%s:%s]$\x1b[0m %s", username, hostname, getcwd(buffer, size), (*current_history)->cmd->seqs[0].cmd_name);
                        index = snprintf(buffer, size, "%s", (*current_history)->cmd->seqs[0].cmd_name);
                    }
                }
                else if (ch == 'B') { // 下箭头
                    CommandHistory* next = next_command_history(*current_history, history_head);
                    if (next) {
                        *current_history = next;
                        printf("\r\x1b[2K"); // 清除当前行
                        printf("\x1b[38;5;208m[xxx@ubunt:%s]$\x1b[0m %s", getcwd(buffer, size), (*current_history)->cmd->seqs[0].cmd_name);
                        index = snprintf(buffer, size, "%s", (*current_history)->cmd->seqs[0].cmd_name);
                    }
                }
                else if (ch == 'C') { // 左箭头
                    move_light_left();
                }
                else if (ch == 'D') { // 右箭头
                    move_light_right();
                }

            }
        }
        else if (ch == 127) { // 处理退格键
            if (index > 0) {
                buffer[--index] = '\0';
                printf("\b \b"); // 删除终端中的最后一个字符
            }
        }
        else {
            buffer[index++] = ch;
            putchar(ch);
        }
    }

    if (index == 0) {
        return -1; // 如果没有输入，返回-1
    }

    buffer[index] = '\0';
    return index;
}

#include <stdio.h>

void move_cursor_left()
{
    // ANSI 转义序列：光标左移一格
    printf("\033[1D");
    fflush(stdout); // 确保输出被立即发送到终端
}

void move_cursor_right()
{
    // ANSI 转义序列：光标右移一格
    printf("\033[1C");
    fflush(stdout); // 确保输出被立即发送到终端
}