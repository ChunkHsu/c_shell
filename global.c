
#include "global.h"
#include <unistd.h>   // getuid
#include <string.h>   // strdup
#include <pwd.h>      // struct passwd and getpwuid
#include <sys/utsname.h> // gethostname
#include <stdio.h> // 为了使用 perror

int DEBUG = 0; // 调试模式

int globalVariable = 0;
COMMAND* global_command = NULL;
char pwd[MAX_PATH_SIZE]; // 用户当前目录

char* username = NULL;
char* hostname = NULL;

/**
 *@brief 获取用户名和主机名
 *
 */
void get_username_and_hostname()
{
    // 获取用户名
    struct passwd* pw = getpwuid(getuid());
    if (pw) {
        username = strdup(pw->pw_name);
    }
    else {
        username = strdup("unknown");
    }

    // 获取主机名
    char host[1024];
    if (gethostname(host, sizeof(host)) == 0) {
        hostname = strdup(host);
    }
    else {
        hostname = strdup("unknown");
    }
}


// 获取当前工作目录
char* get_prompt(char* buffer, size_t bufferSize)
{
    // 使用 getcwd 获取当前工作目录
    if (getcwd(buffer, bufferSize) == NULL) {
        perror("getcwd failed");
        return NULL;
    }
    return buffer;
}


// 检查字符串是否只包含数字
int is_number(const char* str)
{
    if (str == NULL) {
        return 0; // 不是数字
    }

    // 如果字符串为空，也不视为有效的数字
    if (*str == '\0') {
        return 0;
    }

    // 检查字符串中的每个字符
    while (*str) {
        if (!isdigit((unsigned char)*str)) { // 使用 isdigit 检查每个字符
            return 0; // 发现非数字字符
        }
        str++; // 移动到下一个字符
    }

    return 1; // 所有字符都是数字
}