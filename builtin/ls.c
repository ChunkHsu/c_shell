#include "builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

/**
 *@brief 文件格式化输出
 *
 * @param entry  目录项
 * @param path  路径
 * @param longList  是否使用长列表格式
 * @param listAll
 */
void printFileInfo(struct dirent* entry, char* path, int longList, int listAll)
{
    if (!listAll && entry->d_name[0] == '.') {
        return; // Skip hidden files unless -a is specified
    }

    struct stat fileInfo; // 文件信息结构
    char fullPath[1024];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name); // 拼接路径
    if (stat(fullPath, &fileInfo) < 0) { // 获取文件信息
        perror("stat");
        return;
    }

    if (longList) { // 长列表格式
        // 打印权限、链接数、大小和最后修改时间
        printf((S_ISDIR(fileInfo.st_mode)) ? "d" : "-");
        printf((fileInfo.st_mode & S_IRUSR) ? "r" : "-");
        printf((fileInfo.st_mode & S_IWUSR) ? "w" : "-");
        printf((fileInfo.st_mode & S_IXUSR) ? "x" : "-");
        printf((fileInfo.st_mode & S_IRGRP) ? "r" : "-");
        printf((fileInfo.st_mode & S_IWGRP) ? "w" : "-");
        printf((fileInfo.st_mode & S_IXGRP) ? "x" : "-");
        printf((fileInfo.st_mode & S_IROTH) ? "r" : "-");
        printf((fileInfo.st_mode & S_IWOTH) ? "w" : "-");
        printf((fileInfo.st_mode & S_IXOTH) ? "x" : "-");
        printf(" %ld ", fileInfo.st_nlink);
        // Skipping owner/group for simplicity
        printf("%9ld ", fileInfo.st_size);

        char timeStr[256];
        strftime(timeStr, sizeof(timeStr), "%b %d %H:%M", localtime(&fileInfo.st_mtime)); // 格式化时间
        printf("%s ", timeStr); // 最后修改时间
    }

    printf("%s\n", entry->d_name); // 文件名
}

/**
 *@brief 执行ls
 *
 * @param seq
 */
void exec_builtin_ls(SEQ* seq)
{
    int listAll = 0, longList = 0;
    char* path = "."; // 默认路径为当前目录

    for (int i = 0; i < seq->args_count; i++) {
        // 如果是 -a
        if (strcmp(seq->args[i], "-a") == 0) {
            listAll = 1;
        }
        // 如果是 -l
        else if (strcmp(seq->args[i], "-l") == 0) {
            longList = 1;
        }
        // 如果是路径
        else {
            // Assume it's a path
            path = seq->args[i];
        }
    }

    DIR* dir = opendir(path); // 打开目录
    if (dir == NULL) { // 打开失败
        perror("opendir");
        return;
    }

    struct dirent* entry; // 目录项
    while ((entry = readdir(dir)) != NULL) { // 读取目录项
        printFileInfo(entry, path, longList, listAll); // 打印文件信息
    }

    closedir(dir);
}