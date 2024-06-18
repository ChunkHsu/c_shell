#ifndef BUILTIN_H
#define BUILTIN_H

#include "../command.h"  // 修正包含路径
#include "../eval.h"

// 其他内建命令函数声明
void builtin_alias(SEQ* seq);
void builtin_cd(SEQ* seq);
void builtin_cat(SEQ* seq);
void builtin_grep(SEQ* seq);
void builtin_echo(SEQ* seq);
void builtin_type(SEQ* seq);
void builtin_history(SEQ* seq);
void builtin_tab_completion(SEQ* seq);

#endif // BUILTIN_H
