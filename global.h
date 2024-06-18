#ifndef GLOBAL_H
#define GLOBAL_H
#include "command.h"

extern int globalVariable;
extern COMMAND* global_command;
extern char* username;
extern char* hostname;

void get_username_and_hostname();
#endif
