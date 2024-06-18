#include "redir.h"

/**
 *@brief 实现重定向
 *
 * @param cmd seq0 connect seq1
 */
void exec_redir(COMMAND* cmd)
{
    int stdout_copy = dup(STDOUT_FILENO); // 复制标准输出文件描述符
    int stdin_copy = dup(STDIN_FILENO); // 复制标准输入文件描述符
    if (stdout_copy == -1 || stdin_copy == -1) {
        perror("dup");
        return;
    }

    SEQ* command_seq = &cmd->seqs[0]; // 命令序列
    char* redirect_type = cmd->connect; // 重定向类型 > >> < << <<<
    SEQ* file_seq = &cmd->seqs[1]; // 目标文件或字符串


    if (strcmp(redirect_type, ">") == 0) { // 输出重定向
        redirect_output(file_seq->cmd_name);
    }
    else if (strcmp(redirect_type, ">>") == 0) { // 追加重定向
        redirect_append_output(file_seq->cmd_name);
    }
    else if (strcmp(redirect_type, "<") == 0) { // 输入重定向
        redirect_input(file_seq->cmd_name);
    }
    else if (strcmp(redirect_type, "<<") == 0) { // Here Document
        redirect_input(file_seq->cmd_name);
    }
    else if (strcmp(redirect_type, "<<<") == 0) { // Here String
        redirect_input(file_seq->cmd_name);
    }
    else {
        fprintf(stderr, "Unsupported redirection type.\n");
        return;
    }

    // 调用对应的函数执行命令
    exec_cmd(command_seq);

    // 还原 STDOUT_FILENO 与 STDIN_FILENO
    dup2(stdout_copy, STDOUT_FILENO);
    dup2(stdin_copy, STDIN_FILENO);
    close(stdout_copy);
    close(stdout_copy);

}

//todo: << <<<
void do_fork(COMMAND* cmd)
{
    SEQ* command_seq = &cmd->seqs[0]; // 命令序列
    char* redirect_type = cmd->connect; // 重定向类型 > >> < << <<<
    SEQ* file_seq = &cmd->seqs[1]; // 目标文件或字符串

    int file_fd; // 文件描述符
    pid_t pid; // 进程ID

    // 打开目标文件
    file_fd = open(file_seq, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd < 0) { // 打开失败
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 创建子进程
    pid = fork();
    if (pid == -1) { // 创建失败
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // 子进程
        // 重定向标准输出到文件
        if (dup2(file_fd, STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(file_fd); // 关闭不再需要的文件描述符

        // 调用对应的函数执行命令
        exec_cmd(command_seq);

        exit(EXIT_SUCCESS);
    }
    else { // 父进程
        // 等待子进程完成
        wait(NULL);
        close(file_fd);
    }

    return 0;
}


/**
 * @brief > 重定向输出
 *
 * @param filename
 */
void redirect_output(const char* filename)
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    // 重定向标准输出到文件
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

/**
 * @brief >> 追加输出
 *
 * @param filename
 */
void redirect_append_output(const char* filename)
{
    // 实现追加重定向
    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    // 重定向标准输出到文件
    dup2(fd, STDOUT_FILENO);
    close(fd);
}


/**
 *@brief < 重定向输入
 *
 * @param filename
 */
void redirect_input(const char* filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    // 重定向标准输入到文件
    dup2(fd, STDIN_FILENO);
    close(fd);
}