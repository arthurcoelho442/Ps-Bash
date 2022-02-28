#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

int main(){
    int status;
    pid_t return_pid = 0;
    int pid = fork();
    if(pid==0){
        printf("filho indo dormi\n");
        sleep(3);
        printf("filho acordando e finalizando\n");
        return 0;
    }
    else if(pid!=0){
        while(return_pid==0){
            return_pid = waitpid(pid, &status, WNOHANG);
            printf("pai %d esperando pelo filho: %d\n", getpid(), pid);
        }
        printf("pai terminou\n");
    }
    return 0;
}