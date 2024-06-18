#ifndef GLOBAL_H
#define GLOBAL_H
#include "command.h"

#define MAX_PATH_SIZE 1024 * 2

extern int globalVariable;
extern COMMAND* global_command;
extern char* username;
extern char* hostname;
extern char pwd[MAX_PATH_SIZE]; // 用户当前目录

void get_username_and_hostname();

char* get_prompt(char* buffer, size_t bufferSize);
#endif
