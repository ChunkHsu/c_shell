#include "history.h"
#include <stdio.h>
/**
*@brief 添加命令到历史记录
*
* @param head 命令链表的头
* @param tail 命令链表的尾
* @param command 命令
*/
void add_command_to_history(CommandHistory** head, CommandHistory** tail, COMMAND* command)
{
    CommandHistory* new_node = (CommandHistory*)malloc(sizeof(CommandHistory));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    new_node->cmd = command;
    new_node->prev_cmd = *tail;
    new_node->next_cmd = NULL;

    if (*tail != NULL) {
        (*tail)->next_cmd = new_node;
    }
    *tail = new_node;

    if (*head == NULL) {
        *head = new_node;
    }
}

/**
* @brief 打印最近的 n 条历史命令
*
* @param tail 命令链表的尾
* @param n
*/
void print_n_command_history(CommandHistory* tail, int n)
{
    // 从尾部开始打印
    CommandHistory* current = tail;
    int count = 0;
    // 打印最近的 n 条命令
    while (current != NULL && count < n) {
        printf("Command %d: %s\n", count + 1, current->cmd->seqs[0].cmd_name);
        current = current->prev_cmd;
        count++;
    }
}
/**
*@brief 获取上一条历史记录
*
@param current 当前历史记录
@param history_tail 历史记录尾
@return CommandHistory*
*/
CommandHistory* prev_command_history(CommandHistory* current, CommandHistory* history_tail)
{
    if (current == NULL) {
        return history_tail; // 从最新开始
    }
    else {
        return current->prev_cmd;
    }
}
/**
*@brief 获取下一条历史记录
*

@param current 当前历史记录
@param history_head 历史记录头
@return CommandHistory*
*/
CommandHistory* next_command_history(CommandHistory* current, CommandHistory* history_head)
{
    if (current == NULL) {
        return history_head; // 从头开始
    }
    else {
        return current->next_cmd;
    }
}
/**
*@brief 释放 CommandHistory 结构体的内存
*

@param history_head
*/
// 释放 CommandHistory 结构体的内存
void free_command_history(CommandHistory* history)
{
    CommandHistory* current = history;
    CommandHistory* next;

    while (current != NULL) {
        next = current->next_cmd;
        free_command(current->cmd);
        free(current);
        current = next;
    }
}