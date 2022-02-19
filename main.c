#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void type_prompt();
int read_command(char *cmd[100]);
void processInfo(char *msg);
int treatment_command(char cmd[100], char *par[100]);

int main(int argc, char **argv)
{
    char cmd[100], *command[100], *parameters[100];
    int qtdCommand;
    pid_t pid;
    while(1){
        type_prompt();
        qtdCommand = read_command(command);
        if(strcmp(command[0], "term") == 0 && qtdCommand == 1) exit(EXIT_SUCCESS);
        if (qtdCommand == 1)
        { // não vacinado
            if (pid = fork() != 0)
            {
                processInfo("Main");
                wait(NULL);
            }
            else
            {
                int qtdPar=treatment_command(command[0], parameters);
                setpgid(0, 0);
                processInfo("filho");
                strcpy(cmd, command[0]);
                int status=execvp(cmd, parameters);
                if(status == 1) printf("Erro, comando ou parametro errado");
                free(command[0]);
                for(int j=0; j<qtdPar; j++) free(parameters[j]);
                return 0;
            }
        } else if (qtdCommand > 1)
        { // vacinados
            if (pid = fork() != 0)
            {
                processInfo("Deus");
                wait(NULL);
            }
            else
            {
                setpgid(0, 0); // pai
                processInfo("pai");
                int j;
                for (j = 1; j < qtdCommand; j++)
                {
                    pid = fork();
                    if (pid == 0)
                    { // filho
                        int qtdPar=treatment_command(command[j], parameters);
                        processInfo("filho");
                        strcpy(cmd, command[j]);
                        int status=execvp(cmd, parameters);
                        if(status == 1) printf("Erro, comando ou parametro errado");
                        free(command[j]);
                        for(int k=0; k<qtdPar; k++) free(parameters[k]);
                        return 0;
                    }
                }
                int qtdPar=treatment_command(command[0], parameters);
                strcpy(cmd, command[0]);
                int status=execvp(cmd, parameters);
                if(status == 1) printf("Erro, comando ou parametro errado");
                free(command[0]);
                for(int j=0; j<qtdPar; j++) free(parameters[j]);
                wait(NULL);
                return 0;
            }
        }
    }
    return 0;
}

void type_prompt()
{
    static int first_time = 1;
    if (first_time)
    {
        const char *CLEAR_SCREEN_ANSI = " \e[1;1H\e[2J";
        write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
        first_time = 0;
    }
    printf("psh>");
}

int read_command(char *cmd[100])
{
    char linha[1024];
    char *tok;
    int count = 0, i = 0;

    // Pega a linha digitada
    for (;;)
    {
        int c = fgetc(stdin);
        linha[count++] = (char)c;
        if (c == '\n')
            break;
    }
    linha[count]='\0';
    // retorna caso não haja comandos
    if (count == 1)
        return 0;

    
    tok = strtok(linha, ";\n");
    // Divide os comandos caso seja executado mais de um
    while (tok)
    {
        cmd[i++] = strdup(tok);
        tok = strtok(NULL, ";\n");
    }
    return i;
}

void processInfo(char *msg)
{
    pid_t pgid = getpgid(getpid());
    if (pgid == -1)
        perror("getpgid error:");
    pid_t sid = getsid(0);
    if (sid == -1)
        perror("getsid error:");

    printf("%s: This is %d in session %d (created from %d); pgid=%d\n",
            msg, getpid(), sid, getppid(), pgid);
}

int treatment_command(char cmd[100], char *par[100])
{
    int i = 0;
    char *array[100];

    char *tok = strtok(cmd, " ");
    while (tok)
    {
        array[i++] = strdup(tok);
        tok = strtok(NULL, " ");
    }

    strcpy(cmd, array[0]);

    if (i > 1)
    {
        for (int j = 0; j < i; j++)
            par[j] = array[j];
    par[i] = NULL;
    }else {
        par[0]=array[0];
        par[1]=NULL;
    }
    return i;
}