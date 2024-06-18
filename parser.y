%{
void yyerror (char *s);
int yylex();

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "global.h" // 包含全局变量的声明
#include "command.h"

extern int globalVariable;
extern COMMAND* global_command;

#define MAX_ARGS 128

int is_builtin_command(char cmd);
char get_alias_command(char *cmd);
void reset_command_flags();
void allocate_argvtmp(char ***argvtmp, int size);
void free_argvtmp(char ***argvtmp, int size);
%}

%union {
    char* string;
    struct seq* seq;
    struct command* cmd;
}

%start command_line

%token<string> WORD
%token PIPE REDIRECT_IN REDIRECT_OUT ALIAS REDIRECT_OUT_APD REDIRECT_HERE_STR REDIRECT_HERE_DOC EXIT DOLLAR BACKRUN EOL

%type<seq> sequence
%type<cmd> command

%left ';' '\n'
%right '|' '\'

%%

command_line
    : command EOL
        {
            printf("第 %d 个\n", globalVariable++);
            global_command = $1;
            YYACCEPT;
        }
    | EXIT { exit(EXIT_SUCCESS); }
    ;

command
    : sequence
        {
            $$ = init_command();
            $$ = add_command($$, $1, NULL);
        }
    | command PIPE sequence
        { 
            add_command($1, $3, "|");
            set_command_type($1, PipeType);
            $$ = $1;
        }
    | command REDIRECT_OUT sequence
        { /* 设置重定向输出标志 */ 
            add_command($1, $3, ">");
            set_command_type($1, RedirType);
            $$ = $1;
        }
    | command REDIRECT_IN sequence
        { /* 设置重定向输入标志 */ 
            add_command($1, $3, "<");
            set_command_type($1, RedirType);
            $$ = $1;
        }
    | command REDIRECT_OUT_APD sequence
        { /* 设置重定向输出标志 */
            add_command($1, $3, ">>");
            set_command_type($1, RedirType);
            $$ = $1;
        }
    | command REDIRECT_HERE_STR sequence
        {  
            add_command($1, $3, "<<");
            set_command_type($1, RedirType);
            $$ = $1;
        }
    | command REDIRECT_HERE_DOC sequence
        {  
            add_command($1, $3, "<<<");
            set_command_type($1, RedirType);
            $$ = $1;
        }
    | command BACKRUN
        {
            set_command_type($1, BackType);
            $$ = $1;
        }
    ;

sequence
    : WORD
        {
            $$ = init_seq($1);
        }
    | sequence WORD
        {
            $$ = add_seq_args($1, $2);
        }
    ;
%%