
#include "global.h"
#include <unistd.h>   // getuid
#include <string.h>   // strdup
#include <pwd.h>      // struct passwd and getpwuid
#include <sys/utsname.h> // gethostname
#include <stdio.h> // 为了使用 perror


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
