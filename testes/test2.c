#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

/*int main()
{
    int status1;
    int status2;
    pid_t return_pid1 = 0;
    pid_t return_pid2 = 0;
    int pid = fork();
    if (pid == 0)
    {   
        setpgid(0,0);
        printf("filho 1\n");
        int pid2 = fork();
        if (pid2 == 0)
        {
            printf("filho 1-1 indo dormi: %d//%d\n", getpid(), getpgid(getpid()));
            sleep(40);
            printf("filho 1-1 acordando e finalizando %d\n", getpid());
            return 0;
        }
        else if (pid2 != 0)
        {
            while (return_pid1 == 0)
            {
                return_pid1 = waitpid(-1, &status1, WNOHANG);
                //printf("pai %d esperando pelo filho1: %d e return: %d\n", getpid(), pid, return_pid1);
            }
            printf("pai terminou\n");
            exit(EXIT_SUCCESS);
        }
    }
    int pid2 = fork();
    if (pid2 == 0)
    {
        setpgid(0, pid);
        printf("filho 1-2 %d//%d\n", getpid(), getpgid(getpid()));
        sleep(50);
        printf("filho 1-2 acordando e finalizando %d//%d\n", getpid(), getpgid(getpid()));
    }
    waitpid(-1, &status1, 0);
    return 0;
}*/

int main(){
    int status1;
    int status2;
    pid_t return_pid1 = 0;
    pid_t return_pid2 = 0;
    int pid = fork();
    if(pid==0){
        printf("filho indo dormi\n");
        for(int i=0; i<1500; i++){
            
        }
        printf("filho acordando e finalizando %d\n", getpid());
        return 0;
    }

    int pid2 = fork();
    if(pid2==0){
        printf("filho 2 indo dormi\n");
        for(int i=0; i<1000; i++){

        }
        printf("filho 2 acordando e finalizando %d\n", getpid());
        return 0;
    }
    else if(pid!=0){
        printf("PID'S: %d // %d\n", pid, pid2);
        int count=0;
        while(return_pid1>=0){
            return_pid1=waitpid(-1, &status1, WNOHANG);
            printf("pai %d esperando pelo filho1: %d e %d e return: %d//%d\n", getpid(), pid, pid2, return_pid1, return_pid2); 
        }
        return_pid1=waitpid(-1, &status1, WNOHANG);
        printf("pai terminou %d\n", return_pid1);
    }
    return 0;
}