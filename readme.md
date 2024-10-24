# C语音实现linux系统的bash-shell的复刻
  
## 一.	课程设计题目
编写一个词法分析器、语法分析器实现Linux的Shell。以上的程序充当命令行解释器的角色，接受用户命令，通过词法和语法分析器解释这些命令，最后执行它们。
核心功能包括执行基本命令如ls、cd、cat、grep、echo，以及实现type命令查找命令路径，history记录命令历史，alias设置命令别名，tab命令补全，后台运行命令，以及支持管道与重定向等高级功能。
程序采用子进程来执行外部程序，确保父进程可以在子进程执行时继续运行。通过这种方式，Shell能够管理多个任务，实现复杂的命令行操作。

## 二.课程设计内容和要求
查找资料、实现词法分析、语法分析、系统架构、具体功能实现、实验报告， 

## 三. 软件系统设计
采用软件工程的思想方法进行设计，系统分为以下几个模块：
- 解析器模块：分为词法解析器、语法解析器；词法解析器负责将用户输入的命令字符串分解成一系列的标记（tokens），这些标记对应于命令、选项、参数等元素。语法解析器则根据这些标记的顺序和组合，分析命令的结构，确定执行哪个命令以及如何执行。
- 终端显示模块：负责处理与用户界面相关的所有输出，包括命令的输出、错误消息、提示符、命令补全的交互式列表展示等。
- 别名管理模块：用于添加、删除和查找命令别名，允许用户为常用命令定义简短的别名。通过维护一个别名列表，当用户输入一个别名时，该模块将其转换为实际的命令字符串。
- 后台运行模块：允许任务在后台运行，而不占用当前的命令行。当用户在命令末尾添加&符号时，该模块负责创建一个新的进程或线程来执行命令，同时允许用户继续在同一个终端中输入其他命令。
- 重定向模块：处理输入输出重定向，允许命令的输出重定向到文件或从文件中读取输入，包括标准输出（stdout）、标准错误（stderr）和标准输入（stdin）的重定向。
- 管道模块：实现命令之间的数据流连接，允许一个命令的输出直接作为另一个命令的输入。该模块通过创建管道，连接两个或多个命令，使得数据可以在它们之间流动。
- 内置命令模块：包含一组内部实现的命令，如cd、echo、exit等。这些命令直接由Shell程序处理，而不是调用外部程序执行。
- 外部命令模块：负责处理用户输入的非内置命令。该模块首先在系统的PATH环境变量指定的目录中搜索命令的可执行文件，找到后创建一个新的进程执行该命令，如果找不到，则返回错误信息。
### 3.1 整体流程如下：
整个处理流程如下： 

（1）用户在命令行界面输入命令； 

（2）词法解析器进行词法解析生成token； 

（3）语法解析器根据token进行语法解析，根据解析的结果对输入的命令进行校验；若解析不通过，直接跳转到步骤（12）； 

（4）校验成功后，将其分解为命令和参数列表； 

（5）系统跟据参数列表判断是否为后台命令； 

（6）如果为后台命令则新建一个子进程；主进程继续执行shell程序；而子进程后台执行后续流程如第（8）步骤； 

（7）如果非后台命令，则当前主进程根据参数检查命令的类型； 

（8）判断是否为管道、重定向或普通命令； 

（9）而后再检查命令名是否为别名；若为别名则进行实际命令的替换； 

（10）随后检查命令是否为内置命令，如是，则执行builtin目录下的内置命令函数； 

（11）如命令非内置命令，系统去环境变量PATH下查找外部命令，若为外部命令则新建一个子进程执行外部命令；否则提示命令没找到； 

（12）命令处理完毕之后，继续接受新一轮的用户输入。 

 ![image](https://github.com/ChunkHsu/c_shell/assets/92206375/af5a19bc-8505-4b79-a8f2-a49187af9ff4)

整个流程图如上
### 3.2 对象模型
> 对象模型主要分为：
- SEQ结构体：代表单个命令序列，包含命令名称(cmd_name)、参数列表(args)及其数量(args_count)。这是构建命令的基本单元，用于存储单个命令及其参数。
-	COMMAND结构体：代表一个或多个通过管道、重定向或后台执行等方式连接的命令序列。它包含命令序列(seqs)及其数量(seqs_count)、连接符(connect)、命令类型(type)和是否后台执行的标志(is_background)。
### 3.3 动态模型
动态模型展示了程序在运行时的行为，是对象之间的交互。在Shell程序中，动态模型涉及：
- 命令解析：用户输入的命令字符串被解析为SEQ和COMMAND对象。这个过程包括识别命令、参数、连接符（如管道|、重定向>、后台&）。
- 命令执行：根据COMMAND对象的类型和内容，程序决定如何执行命令（直接执行内置命令、创建进程执行外部命令、设置管道、处理重定向等）。
### 3.4 功能模型
功能模型描述了系统提供的功能和这些功能如何实现。对于Shell程序，主要功能包括：
-	命令解释和执行：将用户输入的命令字符串解析为可执行的命令，支持内置命令和外部命令的执行。
-	命令历史、别名：查看执行的历史命令与为命令建立别名，并提供历史命令管理与别名管理功能
-	管道和重定向：支持命令之间的数据流重定向和管道连接。
-	后台执行：允许命令在后台运行，不阻塞当前Shell。
#### 3.4.1 命令解释和执行
将用户输入的命令字符串解析为可执行的命令
> 主要组件
-	词法解析器：将用户输入的命令行文本分解为一系列的tokens，这些词代表命令的不同组成部分，如命令名、参数、重定向符号等；
-	语法解析器：根据词法解析器提供的词序列，构建一个表示命令结构的解析树（parse tree）。这个过程涉及识别命令的语法结构，如管道、重定向、条件执行等，解析成功之后初始化对应的 COMMAND 与SEQ结构；
- 命令查找：确定要执行的命令是内置命令还是外部程序，并在系统中查找相应的可执行文件；
-	环境变量处理：解析命令行中的环境变量引用，并用实际的值替换这些引用；
-	命令执行：实际执行解析和处理后的命令。包括创建进程、设置输入/输出重定向、执行内置或外部命令等；

> 功能流程
1.	接收输入：用户通过键盘输入命令行文本。
2.	解析命令：词法分析与语法分析将输入文本解析为命令和参数。
3.	环境变量和别名处理：替换命令行中的环境变量和别名。
4.	查找命令：在内置命令和系统路径中查找指定的命令。
5.	处理后台命令：若为后台命令，新建一个进程去执行。
6.	处理重定向和管道：在执行命令之前，设置任何指定的输入/输出重定向和管道。
7.	执行命令：
    -	如果是内置命令，直接由Shell执行。
    -	如果是外部命令，Shell创建一个新进程，在该进程中执行命令。
8.	显示结果：将命令的输出显示给标准输出流，或按照重定向和管道的指示进行处理。
9.	返回状态：命令执行完成后，返回状态码，表示成功或失败。

#### 3.4.2 命令历史功能模型
命令历史功能允许用户快速访问之前执行过的命令。这个功能通过记录用户的命令输入历史来实现，用户可以通过特定的快捷键或命令来浏览、搜索和重新执行历史命令。
> 主要组件

•	历史存储：使用双向链表结构，用于存储用户执行的命令历史。
•	历史检索：允许用户搜索历史命令。包括上一条和下一条历史数据、上n条 历史数据、全部历史数据。
•	历史显示：将历史命令输出到标准输出中。
> 功能流程

1.	记录命令：用户输入的每个命令在执行前都被记录到历史存储中。
2.	检索历史：用户使用特定的快捷键或命令来浏览命令历史。
3.	选择和执行：用户可以选择一个历史命令并重新执行。
#### 3.4.3 别名功能模型
别名功能允许用户为长命令或经常使用的命令序列定义一个短的名称（别名）。这样，用户就可以通过输入别名来执行相应的命令，从而提高效率。
> 主要组件

•	别名定义：允许用户为特定的命令序列指定一个别名。
•	别名存储：使用双向链表结构，用于保存所有定义的别名及其对应的命令。
•	别名解析：在执行命令前，Shell会检查输入的命令是否是一个已定义的别名，并将其解析为对应的原始命令序列。
> 功能流程

1.	定义别名：用户通过alias命令定义别名。
2.	存储别名：定义的别名被存储在别名存储中，以便之后使用。
3.	使用别名：用户输入别名作为命令。
4.	解析和执行：Shell检查输入的是否为别名，如果是，则将别名解析为其对应的命令并执行。
#### 3.4.3 管道和重定向
管道允许将一个命令的输出直接作为另一个命令的输入。这意味着多个命令可以连接起来，形成一个处理数据的流水线。
管道是通过‘|’符号表示，它告诉Shell将前一个命令的标准输出（stdout）连接到后一个命令的标准输入（stdin）上
重定向允许用户将命令的输入和输出从默认的位置（通常是终端）重定向到文件或从文件中获取。
-	输入重定向：通过<符号实现，它告诉Shell从指定的文件中读取输入，而不是从标准输入（stdin）读取。
-	输出重定向：通过>或>>符号实现。>将命令的输出写入到指定的文件中，如果文件已存在，则覆盖它；>>也将命令的输出写入到指定的文件中，但如果文件已存在，则将输出追加到文件末尾。
#### 3.4.4 后台执行
后台进程是指那些脱离终端并在后台执行的进程。用户不需要等待进程执行完毕就能回到Shell提示符下继续操作。
启动后台进程：当用户在命令的末尾加上&符号后，Shell解析这个命令，创建一个新的进程来执行指定的命令，并立即返回Shell提示符，不会等待该进程结束。
### 3.5所定义的结构：
**SEQ 命令结构体**
| 属性       | 类型      | 描述       |
|------------|-----------|------------|
| cmd_name   | `char*`   | 命令名称   |
| args       | `char**`  | 命令参数   |
| args_count | `int`     | 参数数量   |

| 方法           | 返回类型 | 描述                      |
|----------------|----------|---------------------------|
| init_seq       | `SEQ*`   | 初始化seq，设置seq的cmd_name |
| add_seq_args   | `SEQ*`   | 给seq添加参数             |
| print_seq      | `void`   | 打印seq                   |
| free_seq       | `void`   | 释放 SEQ 结构体的内存     |

**COMMAND 命令结构体**

| 属性           | 类型      | 描述           |
|----------------|-----------|----------------|
| seqs           | `SEQ*`    | 命令序列       |
| connect        | `char*`   | 命令连接符     |
| seqs_count     | `int`     | 序列数量       |
| type           | `int`     | 命令类型       |
| is_background  | `int`     | 是否是后台命令 |

| 方法                | 返回类型    | 描述                        |
|---------------------|-------------|-----------------------------|
| init_command        | `COMMAND*`  | 初始化命令                  |
| add_command         | `COMMAND*`  | 添加seq序列到command中      |
| set_command_type    | `COMMAND*`  | 设置命令类型                |
| free_command        | `void`      | 释放command内存             |
| print_command       | `void`      | debug函数，打印command      |


**Alias 别名结构体**

| 属性         | 类型            | 描述             |
|--------------|-----------------|------------------|
| alias_name   | `char*`         | 别名             |
| alias_cmd    | `char*`         | 别名对应的原始命令 |
| next         | `struct Alias*` | 下一个别名       |

| 方法                     | 返回类型 | 描述                            |
|--------------------------|----------|---------------------------------|
| add_alias                | `void`   | 添加别名                        |
| get_alias_command        | `const char*` | 打印别名命令                  |
| list_aliases             | `void`   | 列出所有别名                    |
| free_alias_list          | `void`   | 清空别名列表                    |
| change_alias_seq_args    | `SEQ*`   | 将别名转换为对应的原始命令      |
| exec_unalias             | `void`   | 删除别名                        |

**CommandHistory 历史记录结构体**

| 属性             | 类型                                 | 描述           |
| ---------------- | ------------------------------------ | -------------- |
| 存储命令的链表   | `COMMAND*`                           | 存储的命令链表 |
| 上个命令         | `struct CommandHistory*`             | 上一个命令     |
| 下个命令         | `struct CommandHistory*`             | 下一个命令     |

| 方法                          | 返回类型            | 描述                               |
| ----------------------------- | ------------------- | ---------------------------------- |
| add_command_to_history        | `void`              | 添加命令到历史记录                 |
| print_n_command_history       | `void`              | 打印最近的 n 条历史命令            |
| prev_command_history          | `CommandHistory*`   | 获取上一条历史记录                 |
| next_command_history          | `CommandHistory*`   | 获取下一条历史记录                 |
| free_command_history          | `void`              | 清空历史记录                       |


### 3.6所定义的函数：

**eval.h**

| 函数名                         | 功能                   |
|--------------------------------|------------------------|
| `eval_command(COMMAND *cmd)`   | 检查命令类型并执行相应操作 |
| `execute_simple_command(COMMAND* cmd)` | 执行简单命令             |
| `execute_redirection_command(COMMAND* cmd)` | 执行重定向命令           |
| `execute_pipe_command(COMMAND* cmd)` | 执行管道命令             |
| `execute_background_command(COMMAND* cmd)` | 执行后台命令             |

**global.h**

| 函数名                                      | 功能                   |
|---------------------------------------------|------------------------|
| `get_username_and_hostname()`               | 获取用户名和主机名     |
| `get_prompt(char* buffer, size_t bufferSize)` | 获取当前工作目录       |
| `is_number(const char* str)`                | 判断字符串是否是数字   |

**builtin.h**

| 函数名                               | 功能                   |
|--------------------------------------|------------------------|
| `enable_raw_mode()`                  | 开启原始终端模式       |
| `disable_raw_mode()`                 | 关闭原始终端模式       |
| `exec_builtin_alias(SEQ* seq)`       | 执行alias命令          |
| `exec_builtin_cat(SEQ* seq)`         | 执行cat命令            |
| `exec_builtin_cd(SEQ* seq)`          | 执行cd命令             |
| `exec_builtin_echo(SEQ* seq)`        | 执行echo命令           |
| `exec_builtin_grep(SEQ* seq)`        | 执行grep命令           |
| `exec_builtin_history(SEQ* seq)`     | 执行history命令        |
| `exec_builtin_ls(SEQ* seq)`          | 执行ls命令             |
| `exec_builtin_tab_completion(SEQ* seq)` | 执行tab补全命令        |
| `exec_builtin_type(SEQ* seq)`        | 执行type命令           |

**echo.c**

| 函数名                                 | 功能                       |
|----------------------------------------|----------------------------|
| `replace_env_vars(const char* str)`    | 替换字符串中的环境变量     |

**type.c**

| 函数名                                               | 功能                             |
|------------------------------------------------------|----------------------------------|
| `check_command_in_dir(const char* dir, const char* cmd_name, char* fullpath, size_t fullpath_size)` | 检查命令是否在指定目录中         |

**redir.c**

| 函数名                                   | 功能           |
|------------------------------------------|----------------|
| `redirect_output(const char* filename)`  | 重定向输出`>`  |
| `redirect_append_output(const char* filename)` | 追加输出`>>` |
| `redirect_input(const char* filename)`   | 重定向输入`<`  |


## 四.算法设计
### 4.1 词法分析器设计
> 主要算法描述

1.	导入和声明: 导入Yacc生成的头文件和其他必要的头文件，以便能够使用在Yacc文件中定义的TOKEN名称和其他函数。声明yylex和yyerror函数，分别用于执行词法分析和错误处理。
2.	规则定义: 规则部分定义了如何将输入文本分割成tokens。每条规则左边是一个正则表达式，用于匹配输入文本；右边是一个动作，当左边的正则表达式匹配到文本时执行。
3.	忽略空白字符: 空格和制表符被忽略，不会产生任何tokens。
4.	特殊符号处理: 对于特定的符号和关键字（如管道符|、重定向符号>和<、后台运行符号&、环境变量符号$以及exit命令），返回相应的token。这些tokens通常对应于Shell脚本中的控制结构和命令。
5.	字符串处理: 对于被双引号或单引号包围的字符串，将其内容作为一个单独的token，并去除引号。
6.	单词处理: 对于不被特殊符号、空白字符、引号包围的连续字符序列，将其视为一个单词（WORD）。
7.	输入结束处理: 当词法分析器到达输入的末尾时，yywrap函数返回1，表示没有更多的输入需要处理。
### 4.2 语法分析器设计
> 主要算法描述

1.	预处理和全局变量声明: 声明必要的头文件和全局变量，包括用于错误处理的yyerror函数和词法分析器yylex函数。
2.	定义辅助函数和宏，例如判断内置命令、获取别名命令、重置命令标志等。
3.	数据结构定义：使用%union定义了用于语法分析树节点的数据结构，包括字符串和两种自定义结构体seq和command。
4.	语法规则: 定义了Shell命令行输入的语法规则，包括如何处理命令行、命令序列、重定向、管道和后台执行等。
5.	命令行处理: 规定了命令行的结构，支持执行单个命令并处理行结束符EOL，或者处理exit命令以退出程序。
6.	命令处理: 描述了如何处理单个命令、命令序列、以及命令间的管道、重定向和后台执行标志。使用init_command、add_command和set_command_type等函数构建和修改命令结构。
7.	序列处理: 定义了如何处理命令序列，包括单个单词或引用的单词。使用init_seq和add_seq_args等函数创建和扩展命令序列。
> 动作执行

-	对于每条匹配的规则，执行的动作包括初始化命令结构、添加命令参数、设置命令类型等。
-	特别地，对于管道和重定向操作，修改命令类型并增加相应的命令组件。
-	当遇到行结束符EOL时，将构建的命令赋值给全局变量global_command，以便后续执行。
### 4.3 别名管理设计
> 主要算法描述

1.	添加别名 (add_alias): 
  -	首先检查要添加的别名名称是否是内建命令，如果是，则不允许添加，并给出错误提示。
  -	遍历现有的别名列表，如果找到相同的别名，则更新该别名对应的命令。
  - 如果未找到相同的别名，则在链表头部添加新的别名节点。
2.	查找别名对应的命令 (get_alias_command): 
  -	遍历别名列表，如果找到匹配的别名，则返回该别名对应的命令。
  -	如果未找到，则返回NULL。
3.	打印别名命令 (print_alias_command): 遍历别名列表，如果找到匹配的别名，则打印出别名及其对应的命令。
4.	列出所有别名 (list_aliases): 遍历别名列表，打印出所有别名及其对应的命令。
5.	释放别名列表的内存 (free_alias_list): 遍历别名列表，释放每个节点中别名名称和命令所占用的内存，然后释放节点本身。
6.	将别名转换为对应的原始命令 (change_alias_seq_args): 
  -	如果给定的命令序列（SEQ）的命令名称在别名列表中有对应的命令，则创建一个新的命令序列，将别名对应的命令分割成命令名称和参数，并添加到新的命令序列中。
  -	如果别名命令中包含引号，则去除引号。
  -	将原始命令序列中的参数添加到新命令序列的参数列表中。
7.	执行别名和取消别名命令 (exec_builtin_alias): 
  -	根据命令名称判断是执行添加别名操作还是取消别名操作。
  -	对于unalias命令，如果参数是-a，则删除所有别名；否则，删除指定的别名。
  -	对于alias命令，如果没有参数，则列出所有别名；如果有两个参数，第一个参数作为别名名称，第二个参数作为别名对应的命令，然后添加或更新别名。
### 4.4 历史命令管理设计
> 主要算法描述

**添加命令到历史记录 (add_command_to_history):**
1.	创建一个新的CommandHistory节点。
2.	将传入的命令赋值给新节点的cmd字段。
3.	设置新节点的prev_cmd字段为当前链表的尾节点，next_cmd字段为NULL。
4.	如果链表尾节点不为NULL，则将尾节点的next_cmd字段设置为新节点。
5.	更新链表的尾节点为新节点。
6.	如果链表头节点为NULL（即链表为空），则将头节点也设置为新节点。
**打印最近的N条历史命令 (print_n_command_history):**
1.	从链表的尾部开始遍历，逆序打印最近的N条命令。
2.	使用一个计数器来跟踪已打印的命令数量，当达到N时停止。
**获取上一条历史记录 (prev_command_history):**
1.	如果当前节点为NULL，则返回链表的尾节点（最新的命令）。
2.	否则，返回当前节点的prev_cmd节点。
**获取下一条历史记录 (next_command_history):**
1.	如果当前节点为NULL，则返回链表的头节点（最旧的命令）。
2.	否则，返回当前节点的next_cmd节点。
**释放历史记录内存 (free_command_history):**
1.	遍历整个链表，释放每个节点的cmd字段指向的内存（假设free_command是释放命令内存的函数），然后释放节点本身。
2.	使用两个指针（current和next）来遍历链表，以确保节点被正确释放且不丢失对链表的访问。
**执行历史命令 (exec_builtin_history):**
1.	首先，根据传入的命令序列（SEQ）参数数量和类型，决定执行的操作。
2.	如果没有参数，则显示所有历史命令。
3.	如果有一个参数：
  -	如果参数是-c，则清除所有历史记录。
  - 如果参数是数字，表示要显示的历史命令数量，则将该参数转换为整数n，并显示最近的n条命令。
  -	如果参数既不是-c也不是数字，则打印错误信息，提示需要一个数字参数。
4.	如果参数数量大于1，则打印错误信息，提示参数过多。
5.	根据指定的历史命令数量n，计算出应该从哪一条历史命令开始显示，并确保起始位置不会超出历史记录的范围。
6.	遍历历史命令列表，从计算出的起始位置开始，打印每一条历史命令及其编号。
### 4.5 执行重定向命令
> 主要算法描述

1.	保存原始文件描述符: 在进行重定向之前，首先使用 dup 函数复制标准输入（STDIN_FILENO）和标准输出（STDOUT_FILENO）的文件描述符。这样做是为了在重定向操作完成后，能够将标准输入和输出还原到原始状态。
2.	解析重定向类型和目标文件: 
  -	通过 COMMAND 结构体中的 connect 字段识别重定向的类型（>, >>, <, <<, <<<）
  -	根据重定向类型，选择相应的处理函数（redirect_output, redirect_append_output, redirect_input）来执行重定向操作。
3.	执行重定向
  -	输出重定向（>）：使用 open 函数以写入模式打开（或创建）目标文件，然后使用 dup2 将标准输出重定向到该文件。
  -	追加重定向（>>）：与输出重定向类似，不同之处在于文件是以追加模式打开的，这意味着所有写入的数据都会被添加到文件的末尾。
  -	输入重定向（<）：使用 open 函数以只读模式打开目标文件，然后使用 dup2 将标准输入重定向到该文件。
4.	执行命令在重定向设置完成后，调用 exec_cmd 函数执行用户指定的命令。这一步确保了命令的输出或输入将根据之前设置的重定向进行。
5.	还原标准输入/输出
  -	命令执行完成后，使用 dup2 将之前保存的原始标准输入和输出的文件描述符还原回 STDIN_FILENO 和 STDOUT_FILENO。
  -	关闭之前复制的文件描述符以释放资源。
6.	错误处理: 在打开文件、复制文件描述符、执行 dup2 等操作过程中，如果遇到错误，程序会打印错误信息并退出。这是通过检查各个系统调用的返回值来实现的。
### 4.6 执行管道命令
> 主要算法描述

1.	创建管道: 使用 pipe 系统调用创建一个管道。管道是一个简单的内存缓冲区，有两个文件描述符：thepipe[0] 为读端，thepipe[1] 为写端。
2.	创建第一个子进程: 使用 fork 创建第一个子进程。在这个子进程中，执行管道的左侧命令（cmd->seqs[0]）。
3.	在第一个子进程中重定向输出: 
  -	关闭管道的读端（thepipe[0]），因为这个子进程只需要向管道写数据。
  -	使用 dup2 将标准输出（STDOUT_FILENO，即文件描述符 1）重定向到管道的写端（thepipe[1]）。这样，这个子进程的输出就会发送到管道中。
  -	关闭管道的写端（thepipe[1]），因为在 dup2 后，标准输出已经是管道的写端了。
  -	执行命令（通过 exec_cmd）。
4.	创建第二个子进程: 再次使用 fork 创建第二个子进程。在这个子进程中，执行管道的右侧命令（cmd->seqs[1]）。
5.	在第二个子进程中重定向输入
  -	关闭管道的写端（thepipe[1]），因为这个子进程只需要从管道读数据。
  -	使用 dup2 将标准输入（STDIN_FILENO，即文件描述符 0）重定向到管道的读端（thepipe[0]）。这样，这个子进程的输入就会来自于管道。
  -	关闭管道的读端（thepipe[0]），因为在 dup2 后，标准输入已经是管道的读端了。
  -	执行命令（通过 exec_cmd）。
6.	在父进程中关闭管道: 在创建了两个子进程后，父进程中不再需要管道，因此关闭管道的两个端口。
7.	父进程等待子进程完成: 使用 waitpid 等待两个子进程依次完成。对于每个子进程，检查其退出状态，以确定它是正常退出、被信号杀死，还是异常退出。
### 4.7	 执行后台命令
> 主要算法描述

1.	创建子进程：使用 fork 系统调用创建一个新的子进程。fork 创建了父进程的一个副本，子进程和父进程从 fork 调用之后的代码开始执行。
2.	错误处理：如果 fork 返回一个负值，这表示创建子进程失败了。此时，会使用 perror 打印错误信息。
3.	子进程执行：
  -	在子进程中（fork 返回 0），首先将 COMMAND 结构体中的 is_background 标志设置为 0，表示这个命令不应作为后台进程运行。这是因为在子进程中，该命令实际上是在前台执行的，尽管对于父进程（Shell）来说，它是在后台运行。
  -	然后，调用 eval_command（或类似的函数）执行实际的命令。这个函数负责解析 COMMAND 结构体中的信息并执行相应的命令。
  -	如果命令执行失败，使用 _exit(EXIT_FAILURE) 确保子进程退出。这里使用 _exit 而不是 exit 是因为 _exit 直接终止进程，不会调用任何注册的退出处理函数或刷新标准 I/O 缓冲区，这在子进程中通常是所期望的行为。
4.	父进程执行：
  -	在父进程中（fork 返回子进程的 PID），打印出子进程的 PID，并返回到 Shell 提示符。父进程不会等待子进程结束，这就允许了子进程在后台运行。
  -	父进程不需要特别处理子进程的状态，因为子进程在执行完毕后会自动退出。如果需要，可以通过设置一个信号处理函数来处理子进程的结束，以避免产生僵尸进程。
4.8 执行内置与外部命令
> 主要算法描述

1.	系统跟据参数列表判断是否为后台命令；
2.	如果为后台命令则新建一个子进程；主进程继续执行shell程序；而子进程后台执行后续流程如第（4）步骤；
3.	如果非后台命令，则当前主进程根据参数检查命令的类型；
4.	判断是否为管道、重定向或普通命令；
5.	而后再检查命令名是否为别名；若为别名则进行实际命令的替换；
6.	随后检查命令是否为内置命令，如是，则执行builtin目录下的内置命令函数；
7.	如命令非内置命令，系统去环境变量PATH下查找外部命令，若为外部命令则使用fork创建子进程，并在子进程中使用execvp执行外部命令；否则提示命令没找到；
8.	命令处理完毕之后，继续接受新一轮的用户输入
### 4.9 代码补全
> 主要算法描述

1.	判断参数列表个数如果大于0则取第一个参数作为需要补全的文本
2.	检查当前文本是否含有’/’符号
  -	如果含有则是路径补全
  -	根据最后一个’/’提取出目录路径find_path和需要补充的前缀prefix，然后在指定目录下寻找与前缀匹配的文件或目录;
  -	如果前缀匹配则输出改文件名或目录名
3.	如果没有则是命令补全
-	列出环境变量下的可执行文件名与内置命令文件名，而后，遍历每个文件与前缀prefix进行匹配；
-	如果匹配则输出；
-	不匹配继续比较下一个直至结束； 

## 五.小结
1. 课程设计中遇到的问题及解决办法
   
（1）在使用yacc编写的语法分析器与主函数main.c进行融合时，出错Segmetn Default（core dump）严重错误！ 

解决方法：因为在终端输入到语法分析器时其回车的换行符号不会添加到输入字符串input后面，而语法解析器的终止符是换行符号，所以对于input，语法解析器无法正常终止；需要手动在input后添加一个换行符号strcpy(modifiedInput, input);       strcat(modifiedInput, "\n"); 便可以解决问题。

（2）在接收char类型的字符指针时报错Segmetn Default（core dump）严重错误！ 

解决方法：因为在使用改函数时，可能将NULL空指针赋值给char指针，而在函数内部使用想strcpy()对char指针进行了复制，所以当NULL空指针赋值给char指针时，strcpy()会出错。所以添加判断条件当赋值的char不为NULL时进行strcpy()，便可解决问题。

（3）后台进程的输出默认情况下会发送到控制终端，这可能会干扰到用户当前的工作或导致输出混乱。 

解决办法：可以通过重定向后台进程的标准输出到其他文件，以避免输出到控制终端。


2. 课程设计还存在哪些问题。
（1）别名方面：在实现别名时是通过一个双向链表来实现的，在处理大量别名时效率较低，且别名定义没有考虑到递归别名的情况，可以哈希表来存储别名，此外在解析别名时如果检测到递归的别名应该使用一个链表去链接到其中的内部的别名。

（2）命令参数处理方面：如果使用了多个参数如 cat -n -s 目前只支持分开写参数，且参数在最前面对于cat -ns不能识别；改进的思路是分割参数字符串，将输入的命令行参数分割成单独的标记，而后遍历参数，如果遇到一个参数以 - 开头，将其视为一个选项，这个选项可能是单个字符（如 -n），也可能是多个字符组合（如 -ns）；之后解析组合参数，对于组合参数（如 -ns），可以进一步将其分解为单独的字符，并分别处理。 

（3）没有支持多命令，目前只能执行一行命令，支持执行多命令需要修改语法分析器，而后命令执行方面的顺序也需要修改，且需要更改对于逻辑，设置多个COMMAND来分析。 

（4）目前所有命令的输出基本都是输出到标准输出里面stdout，不利于模块化，后续可以设置一个特定的文件描述符中；也可以增加日志和调试支持

3. 本课程设计有哪些收获和心得体会。
通过本次课程实践，实现一个shell，不仅可以深入理解操作系统的工作原理，特别是进程管理、文件描述符、环境变量等概念，而且还能够加深对编程语言和数据结构应用的理解。以下是从这个项目中获得的一些主要收获和心得体会：
理解操作系统概念方面： 进程管理：实现shell时，深入了解了进程的创建fork、执行exec系列函数和进程间通信pipe。文件描述符和重定向：通过实现输入输出重定向，深入理解了文件描述符的概念以及标准输入、输出、错误的工作机制。学习如何使用dup2函数将标准输出重定向到文件。信号处理：实现了对特定信号的捕捉和处理，这不仅增强了对操作系统如何管理进程的理解，还学会了如何编写更加健壮的程序来处理用户的中断请求或者子进程的状态变化。
编程技能的提升方面：字符串和指针操作：在解析命令行输入、处理环境变量时，大量使用了字符串和指针操作，这对于提升C语言编程技能非常有帮助。学会了如何安全、高效地处理字符串，避免常见的内存泄露和缓冲区溢出问题。数据结构的应用：通过使用链表管理历史命令、存储别名等，加深了对这些基本数据结构的理解和应用。这不仅提高了程序的效率，还让我对如何选择合适的数据结构以解决特定问题有了更深入的认识。
软件工程实践方面：模块化和代码重用：在设计shell时，我学会了如何将程序分解成独立、可重用的模块，比如命令解析、执行、内建命令处理等。这种模块化思想不仅使代码更加清晰，也提高了代码的可维护性和扩展性。同时，也意识到了在程序中进行适当的错误检查和处理的重要性。
实现shell项目是一次全面的学习经历，它不仅让我深入理解了操作系统的工作原理，还提高了我的编程技能，尤其是在C语言、数据结构和软件工程实践方面。更重要的是，通过这个项目，我学会了如何面对复杂问题，分解问题，并逐步解决。

## 六.参考文献
[1] (美)Bruce Molay著 杨宗源 黄海涛译 UnixLinux编程实践教程. 

[2] Bruce Perens. Linux系统基础知识. 

[3] Daniel P. Bovet, Marco Cesati. 深入理解Linux内核. 

[4] UNIX环境高级编程(第3版) (Advanced Programming in the UNIX Environment, 3rd Edition) (史蒂文斯 (W. Richard Stevens), Stephen Rago) . 

[5] home_king．详解Bash命令行处理，2005. 

[6] Eric S. Raymond．Unix编程艺术，北京：电子工业出版社，2006.




# 使用方法
本代码在`ubuntu20.04`编译运行，使用`gcc` 

词法分析器使用 `flex`编译，需要ubuntu下载 `flex` 

语法分析器使用 `bison`编译，需要ubuntu下载 `bison` 

需要下载readline库 

使用`Makefile`的`make`命令编译出`parser`可执行文件，运行即可 

可以使用如下命令将其设置为默认启动shell 

> chmod +x "/path/to/parser"
> 
> echo "path/to/parser" | sudo tee -a /etc/shells
> 
> chsh -s "path/to/parser" 
