#include "command.h"
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

/**
 *@brief 初始化seq，设置seq的cmd_name
 *
 * @param cmd_name
 * @return SEQ*
 */
SEQ* init_seq(char* cmd_name)
{
    SEQ* seq = (SEQ*)malloc(sizeof(SEQ));
    if (seq == NULL) return NULL;
    seq->cmd_name = strdup(cmd_name);
    if (seq->cmd_name == NULL) {
        free(seq);
        return NULL;
    }
    seq->args = (char**)malloc(sizeof(char*));
    if (seq->args == NULL) {
        free(seq->cmd_name);
        free(seq);
        return NULL;
    }
    seq->args[0] = NULL; // 初始化为空的参数列表
    seq->args_count = 0;
    return seq;
}

/**
 *@brief 给seq添加参数
 *
 * @param seq 需要添加参数的seq
 * @param arg 添加的参数
 * @return SEQ*
 */
SEQ* add_seq_args(SEQ* seq, char* arg)
{
    char** new_args = (char**)realloc(seq->args, (seq->args_count + 2) * sizeof(char*)); // +2 because we need space for new arg and the NULL terminator
    if (new_args == NULL) {
        return seq; // 保持旧的指针，不改变
    }
    seq->args = new_args;
    seq->args[seq->args_count] = strdup(arg);
    if (seq->args[seq->args_count] == NULL) {
        return seq; // strdup 失败时不改变 args 数组
    }
    seq->args_count++;
    seq->args[seq->args_count] = NULL; // 确保参数列表以NULL结尾
    return seq;
}

/**
 *@brief 释放 SEQ 结构体的内存
 *
 * @param seq
 */
void free_seq(SEQ* seq)
{
    if (seq->cmd_name != NULL) {
        free(seq->cmd_name);
    }
    if (seq->args != NULL) {
        for (int i = 0; i < seq->args_count; i++) {
            free(seq->args[i]);
        }
        free(seq->args);
    }
    free(seq);
}

/**
 *@brief 初始化
 *
 * @return COMMAND*
 */
COMMAND* init_command()
{
    COMMAND* command = (COMMAND*)malloc(sizeof(COMMAND));
    if (command == NULL) return NULL;
    // 不再在这里为 seqs 分配内存，因为我们会在 add_command 中动态添加 SEQ
    command->seqs = NULL; // 初始化为 NULL，稍后在 add_command 中分配
    command->seqs_count = 0;
    command->type = SimpType; // 默认为普通命令
    command->connect = NULL;
    command->is_background = 0;
    return command;
}

/**
 *@brief 添加seq序列到command中
 *
 * @param command
 * @param seq
 * @return COMMAND*
 */
COMMAND* add_command(COMMAND* command, SEQ* seq, char* connect)
{
    command->seqs_count++;
    // 每次添加 SEQ 时都重新分配 seqs 数组的内存
    SEQ* new_seqs = (SEQ*)realloc(command->seqs, command->seqs_count * sizeof(SEQ));
    if (new_seqs == NULL) {
        return NULL; // realloc 失败时返回 NULL
    }
    command->seqs = new_seqs;
    command->seqs[command->seqs_count - 1] = *seq; // 注意：这里可能需要深拷贝

    if (connect != NULL) {
        // 复制连接符号字符串
        if (command->connect != NULL) {
            free(command->connect);
        }

        command->connect = strdup(connect);
        if (command->connect == NULL) return NULL; // 检查strdup是否成功
    }

    return command;
}

/**
 *@brief 设置命令类型
 *
 * @param command
 * @param type
 * @return COMMAND*
 */
COMMAND* set_command_type(COMMAND* command, int type)
{
    command->type |= type;
    if (type == BackType) {
        command->is_background = 1;
    }
    return command;
}

/**
 *@brief 释放command内存
 *
 * @param cmd
 */
void free_command(COMMAND* cmd)
{
    if (cmd == NULL) return;

    // 释放每个命令序列
    for (int i = 0; i < cmd->seqs_count; i++) {
        SEQ* seq = &(cmd->seqs[i]);

        // 释放每个参数字符串
        for (int j = 0; j < seq->args_count; j++) {
            free(seq->args[j]);
        }

        // 释放参数数组本身
        free(seq->args);
    }

    // 释放命令序列数组
    free(cmd->seqs);

    // 释放连接符字符串
    if (cmd->connect) {
        free(cmd->connect);
    }

    // 释放 COMMAND 结构体本身
    free(cmd);
}

/**
 *@brief debug函数，打印command
 *
 * @param global_command
 */
void print_command(COMMAND* global_command)
{
    if (global_command == NULL)
        printf("global_command is NULL\n");
    else {
        printf("总共有 %d 个命令序列\n", global_command->seqs_count);
        for (int i = 0; i < global_command->seqs_count; i++) {
            SEQ* seq = &(global_command->seqs[i]);
            printf("命令 %d: %s\n", i + 1, seq->cmd_name);
            printf(" 参数: ");
            for (int j = 0; j < seq->args_count; j++) {
                printf("%s ", seq->args[j]);
            }
            printf("\n");
        }
        printf("连接符: %s\n", global_command->connect ? global_command->connect : "无");
        printf("是否后台: %d\n", global_command->is_background);
    }
}
