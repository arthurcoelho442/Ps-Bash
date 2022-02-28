#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

void type_prompt();
int read_command(char **cmd);
void processInfo(char *msg);
int treatment_command(char* cmd, char **par, int* direcionaSaida, char *nameFile);
int naoVacinado(pid_t pid, int* direcionaSaida, char* commad, char** parameters, char* nameFile);
int vacinados(char** command, int qtdCommand, char** parameters, int* direcionaSaida, char* nameFile);
void psNaoMorrer(int num){
    write(STDERR_FILENO, "\nEstou vacinada...desista!!\n", 28);
}
void novaCepa(){
    char* text = "\n                       ,---.\n                       /    |\n                      /     |\n                     /      |\n                    /       |\n               ___,'        |\n             <  -'          :\n              `-.__..--'``-,_\'_\n                 |o/ ` :,.)_`>\n                 :/ `     ||/)\n                 (_.).__,-` |\\n                 /( `.``   `| :\n                 \'`-.)  `  ; ;\n                 | `       /-<\n                 |     `  /   `.\n ,-_-..____     /|  `    :__..-'\\n/,'-.__\\  ``-./ :`      ;       \\n`\' `\'  `\\  \' :  (   `  /  ,   `. \\n  \'` \'   \\   |  | `   :  :     .\' \\n   \' `\'_  ))  :  ;     |  |      ): :\n  (`-.-'\' ||  |\' \'   ` ;  ;       | |\n   \'-_   `;;._   ( `  /  /_       | |\n    `-.-.// ,'`-._\'__/_,'         ; |\n       \':: :     /     `     ,   /  |\n        || |    (        ,' /   /   |\n        ||                ,'   /    |\n________ Unfortunately all process died!________\n___ Vaccination should be a social contract!____\n____Cooperation was the morally right choice!___\n";
}
int main(int argc, char **argv){
    signal(SIGINT, psNaoMorrer);
    signal(SIGQUIT, psNaoMorrer);
    signal(SIGTSTP, psNaoMorrer);
    signal(SIGUSR1, novaCepa);
    
    char cmd[100], *command[100], *parameters[100];
    int qtdCommand;
    pid_t pid;
    int saida, saveOut;
    char nameFile[100];
    int direcionaSaida[1];
    direcionaSaida[0] = 0;

    while(1){
        type_prompt();
        qtdCommand = read_command(command);
        
        if(strcmp(command[0], "term") == 0 && qtdCommand == 1)
            exit(EXIT_SUCCESS);
        if (qtdCommand == 1){ // não vacinado
            int qtdPar=treatment_command(command[0], parameters, direcionaSaida, nameFile);
            switch(naoVacinado(pid, direcionaSaida, parameters[0], parameters, nameFile)){
                case 1:
                    perror("Não foi possivel abrir nem criar o arquivo");
                    break;
                case 2:
                    perror("Não foi possivel redirecionar a saida");
                    break;
                case 3:
                    perror("Não foi possivel redirecionar a saida padrao");
                    break;
                case 4:
                    printf("Erro, comando ou parametro errado");
                    break;
            }
            for(int j=0; j<qtdPar; j++)
                free(parameters[j]);
        } else if (qtdCommand > 1){ // vacinados
            switch(vacinados(command, qtdCommand, parameters, direcionaSaida, nameFile)){
                case 1:
                    perror("Não foi possivel abrir nem criar o arquivo");
                    break;
                case 2:
                    perror("Não foi possivel redirecionar a saida");
                    break;
                case 3:
                    perror("Não foi possivel redirecionar a saida padrao");
                    break;
                case 4:
                    printf("Erro, comando ou parametro errado");
                    break;
                case 5:
                    perror("Não foi possivel instalar os sinais");
                    break;
                case 6: 
                    perror("Não foi possivel bloquear os sinais instalados");
                    break;
            }
        }
    }
    return 0;
}

void type_prompt(){
    static int first_time = 1;
    if (first_time)
    {
        const char *CLEAR_SCREEN_ANSI = " \e[1;1H\e[2J";
        write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
        first_time = 0;
    }
    printf("psh>");
}

int read_command(char **cmd){
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

void processInfo(char *msg){
    pid_t pgid = getpgid(getpid());
    if (pgid == -1)
        perror("getpgid error:");
    pid_t sid = getsid(0);
    if (sid == -1)
        perror("getsid error:");

    printf("%s: This is %d in session %d (created from %d); pgid=%d\n",
            msg, getpid(), sid, getppid(), pgid);
}

int treatment_command(char *cmd, char **par, int* direcionaSaida, char *nameFile){
    int i = 0;
    char *array[100];

    char *tok = strtok(cmd, ">");
    cmd = strdup(tok);
    tok = strtok(NULL, ">");
    if(tok){
        array[0] = strdup(tok);
        tok = strtok(array[0], " ");
        array[0] = strdup(tok);
        strcpy(nameFile, array[0]);
        direcionaSaida[0]=1;
    }
    tok = strtok(cmd, " ");
    while (tok)
    {
        array[i++] = strdup(tok);
        tok = strtok(NULL, " ");
    }
    for (int j = 0; j < i; j++)
        par[j] = array[j];
    par[i] = NULL;
    return i;
}

int naoVacinado(pid_t pid, int* direcionaSaida, char* commad, char** parameters, char* nameFile){
    int status;
    int saida, saveOut;
    
    if (pid = fork() != 0){
        processInfo("Main");
        waitpid(-1, &status, WNOHANG);  //BACKGROUND
    }
    else{
        //Redirecionamento da Saida
        if(direcionaSaida[0]){
            saida = open(nameFile, O_APPEND | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
            if(saida == -1)
                return 1;

            saveOut = dup(fileno(stdout));

            if(dup2(saida, 1) == -1)
                return 2;
            close(saida);
        }
        //////////////////////////
        setpgid(0, 0);
        processInfo("filho");
        status=execvp(commad, parameters);
        if(status == 1)
            return 4;

        if(direcionaSaida[0]){
            dup2(saveOut, 1);
            close(saveOut);
            direcionaSaida[0]=0;
            return 3;
        }
        return 0;
    }
}
int vacinados(char** command, int qtdCommand, char** parameters, int* direcionaSaida, char* nameFile){
    pid_t pid;
    int saida, saveOut;
    if (pid = fork() != 0){
        processInfo("Deus");
        int status;
        
        waitpid(-1, &status, WNOHANG);  //BACKGROUND
        
    }
    else {
        sigset_t newsigset;
        if((sigemptyset(&newsigset) == -1) || (sigaddset(&newsigset, SIGINT) == -1) || (sigaddset(&newsigset, SIGQUIT) == -1) 
                || (sigaddset(&newsigset, SIGTSTP) == -1))
            return 5;
        if(sigprocmask(SIG_BLOCK, &newsigset, NULL) == -1)
            return 6;
        
        setpgid(0, 0); // pai
        processInfo("pai");
        int j;
        for (j = 1; j < qtdCommand; j++){
            pid = fork();
            if (pid == 0){ // filho
                int qtdPar=treatment_command(command[j], parameters, direcionaSaida, nameFile);
                //Redirecionamento da Saida
                if(direcionaSaida[0]){
                    saida = open(nameFile, O_APPEND | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
                    if(saida == -1)
                        return 1;

                    saveOut = dup(fileno(stdout));

                    if(dup2(saida, fileno(stdout)) == -1)
                        return 2;

                    close(saida);
                }
                //////////////////////////
                processInfo("filho");
                int status=execvp(parameters[0], parameters);
                if(status == 1) 
                    return 4;
                for(int k=0; k<qtdPar; k++) 
                    free(parameters[k]);
                return 0;
                if(direcionaSaida[0]){
                    dup2(saveOut, fileno(stdout));
                    close(saveOut);
                    direcionaSaida[0]=0;
                }
            }
        }
        int qtdPar=treatment_command(command[0], parameters, direcionaSaida, nameFile);
        //Redirecionamento da Saida
        if(direcionaSaida[0]){
            saida = open(nameFile, O_APPEND | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
            if(saida == -1){
                perror(nameFile);
                return 0;
            }

            saveOut = dup(fileno(stdout));

            if(dup2(saida, fileno(stdout)) == -1){
                perror("Não foi possivel redirecionar a saida \n");
                return 0;
            }
            close(saida);
        }
        //////////////////////////
        int status=execvp(parameters[0], parameters);
        if(status == 1) 
            printf("Erro, comando ou parametro errado");
        for(int j=0; j<qtdPar; j++) free(parameters[j]);
        if(direcionaSaida[0]){
            dup2(saveOut, fileno(stdout));
            close(saveOut);
            direcionaSaida[0]=0;
        }
        waitpid(-1, &status, WNOHANG);  //BACKGROUND
        return 0;
    }
} 