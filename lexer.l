%{
#include "y.tab.h" // 包含Yacc生成的头文件，确保TOKEN的名称匹配   
#include "command.h"
int yylex();
void yyerror(char *s); // 处理错误的函数，需要在Yacc文件中定义
%}

%%
[ \t]+              ; // 忽略空格和制表符
[\n]               { return EOL; } // 行结束，返回EOL表示输入结束
"|"                 { return PIPE; } // 管道符号 

">>"                {return REDIRECT_OUT_APD;}
"<<"                {return REDIRECT_HERE_STR;}
"<<<"               {return REDIRECT_HERE_DOC;}
">"                 {return REDIRECT_OUT; } // 重定向符号
"<"                 {return REDIRECT_IN; } // 反向重定向符号
"&"                 {return BACKRUN; } // 后台运行符号
"$"                 {return DOLLAR; } // 环境变量符号
"exit"              {return EXIT; } // 退出shell

[^ \t\n|<>]+        { yylval.string = strdup(yytext); return WORD; } // 单词

%%
int yywrap() {
    return 1; // 表示输入结束
}

