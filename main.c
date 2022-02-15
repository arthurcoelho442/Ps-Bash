#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>

int read_command(char *cmd[100]){
    char linha[1024];
    char *tok;
    int count =0, i=0;

    //Pega a linha digitada
    for(;;){
        int c = fgetc(stdin);
        linha[count++] = (char) c;
        if( c == '\n' ) break;
    }
    
    //retorna caso não haja comandos
    if(count == 1) 
        return 0;
    
    //Divide os comandos caso seja executado mais de um
    tok = strtok(linha, ";");
    while(tok){
        cmd[i++] = strdup(tok);
        tok = strtok (NULL, ";");
    }
    
    return i;
}

void treatment_command(char cmd[], char *par[]){ 
    int i = 0;
    char *array[100];
    char *tok;
    
    if(cmd[strlen(cmd)-1] == '\n')
        cmd[strlen(cmd)-1] = '\0';
    
    tok = strtok(cmd, " ");
    while(tok != NULL){
        array[i++] = strdup(tok);
        tok = strtok (NULL, " ");
    }
    
    strcpy(cmd, array[0]);
    for(int j=0; j < i; j++)
        par[j] = array[j]; 
    par[i] = NULL;
}

void type_prompt(){
    static int first_time = 1;
    if(first_time){
        const char* CLEAR_SCREEN_ANSI =" \e[1;1H\e[2J";
        write(STDOUT_FILENO,CLEAR_SCREEN_ANSI, 12);
        first_time=0;
    }
    printf("psh>");
}

int main(int argc, char **argv){
    char cmd[100], *command[100], *parameters[100];
    char *envp[]={(char*) "PATH=/bin", 0};
    int qtdCommand;
    pid_t pid;
    
    while(1){
        type_prompt();
        qtdCommand = read_command(command);
        
        for(int i=0; i<qtdCommand; i++){
            treatment_command(command[i], parameters);
            if(pid = fork()!=0){
                //printf("\nGPP = %d\n", getpgid(pid));
                wait(NULL);
            }
            else{
                if(qtdCommand == 1)//não vacinado
                    setpgid(0,0); 
                //printf("\nGPF = %d\n", getpgid(pid));
                strcpy(cmd, "/bin/");
                strcpy(cmd, command[i]);
                execvp(cmd, parameters);
            }
            if(strcmp(command[i], "exit")==0) return 0;
        }
    }
    return EXIT_SUCCESS;
}