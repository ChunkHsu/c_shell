
#include "global.h"
#include <unistd.h>   // getuid
#include <string.h>   // strdup
#include <pwd.h>      // struct passwd and getpwuid
#include <sys/utsname.h> // gethostname


int globalVariable = 0;
COMMAND* global_command = NULL;

char* username = NULL;
char* hostname = NULL;

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