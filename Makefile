# 输入make ls 编译 builtin/do_ls.cpp -> bin/ls
CC = gcc
CFLAGS = -Wall -g -I. -Ibuiltin -Iredir -Ibackrun -Ipipe # 包含当前目录、builtin目录、redir目录
LDFLAGS = -lfl -lreadline -Wno-unused-variable -Wno-unused-function
SRC = main.c command.c history.c input.c global.c eval.c builtin/alias.c \
	builtin/cd.c builtin/cat.c builtin/grep.c builtin/echo.c \
	builtin/type.c builtin/history.c builtin/tab.c \
	builtin/builtin.c redir/redir.c backrun/backrun.c  pipe/pipe.c


all: parser

# % 是一个通配符
# $< 是一个自动变量，表示规则的第一个依赖，也就是相应的 .cpp 文件
# $(@) 是一个自动变量，表示规则的目标
# % : $(BUILTIN)/do_%.cpp
# 	$(CC) $< -o $(BIN)/$(@)  



# 语法分析的
parser: lex.yy.c y.tab.c
	# gcc -o parser main.c command.c history.c input.c global.c \
	# eval.c y.tab.c lex.yy.c -lfl -lreadline
	$(CC) $(CFLAGS) -o parser $(SRC) y.tab.c lex.yy.c $(LDFLAGS)

lex.yy.c: y.tab.c lexer.l
	# flex --header-file=lex.yy.h lexer.l
	lex lexer.l

y.tab.c: parser.y
	yacc -d parser.y

