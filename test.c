#include<stdio.h>
#include<unistd.h> // 引入unistd.h以使用sleep()

int main()
{
    int count = 0; // 初始化计数器
    int limit = 10; // 设置循环的次数

    while (count < limit) // 循环直到达到指定的次数
    {
        printf("Hello, World! %d\n", count + 1); // 打印当前计数（从1开始）
        sleep(1); // 程序暂停1秒
        count++; // 增加计数器
    }
    printf("Hello, World!\n");
    return 0;

}
