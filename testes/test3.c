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
        if (pid != 0){//Pai
            pid = fork();
        }else {//Filho
            sleep(20);
            return 0;
        }
    }
    if(pid != 0){
        sleep(10);
        killpg(getpid(), SIGUSR1);
    }
    return 0;
}
//Finalizado