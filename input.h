#ifndef INPUT_H
#define INPUT_H

#include "history.h"

#define BUFFER_SIZE 1024

extern char* username;
extern char* hostname;

void enable_raw_mode();
void disable_raw_mode();
int get_input(char* buffer, int size, CommandHistory** current_history, CommandHistory* history_head, CommandHistory* history_tail);

#endif // INPUT_H