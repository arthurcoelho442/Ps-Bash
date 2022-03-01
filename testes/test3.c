#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

int main()
{
    int pid = fork();
    for (int i = 0; i < 10; i++)
    {
        if (pid != 0)
        {
            pid = fork();
        }else {
            sleep(60);
            return 0;
        }
    }
    return 0;
}