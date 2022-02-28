#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

/*int main(){
    int status1;
    int status2;
    pid_t return_pid1 = 0;
    pid_t return_pid2 = 0;
    int pid = fork();
    if(pid==0){
        printf("filho indo dormi\n");
        sleep(50);
        printf("filho acordando e finalizando %d\n", getpid());
        return 0;
    }
    
    int pid2 = fork();
    if(pid2==0){
        printf("filho 2 indo dormi\n");
        sleep(50);
        printf("filho 2 acordando e finalizando %d\n", getpid());
        return 0;
    }
    else if(pid!=0){
        printf("PID'S: %d // %d\n", pid, pid2);
        while(return_pid1==0 || return_pid2==0){
            return_pid1 = waitpid(pid, &status1, WNOHANG);
            return_pid2 = waitpid(pid2, &status2, WNOHANG);
            printf("pai %d esperando pelo filho1: %d e %d e return: %d//%d\n", getpid(), pid, pid2, return_pid1, return_pid2);
        }
        printf("pai terminou\n");
    }
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
        sleep(50);
        printf("filho acordando e finalizando %d\n", getpid());
        return 0;
    }
    
    int pid2 = fork();
    if(pid2==0){
        printf("filho 2 indo dormi\n");
        sleep(50);
        printf("filho 2 acordando e finalizando %d\n", getpid());
        return 0;
    }
    else if(pid!=0){
        printf("PID'S: %d // %d\n", pid, pid2);
        waitpid(-1, &status1, WNOHANG);
        printf("pai %d esperando pelo filho1: %d e %d e return: %d//%d\n", getpid(), pid, pid2, return_pid1, return_pid2);
        printf("pai terminou\n");
    }
    return 0;
}