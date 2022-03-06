#include "processos.h"
process* inicProcess(int pid, int idt){
    process* p = (process*) malloc(sizeof(process));//aloca o processo dinamicamente
    p->pid = pid;//coloca o pid no processo
    p->identify = idt;//coloca o id de vacinado ou não vacinado
    p->prox = NULL;//define o proximo como nulo
    return p;//retorna o processo
}
void freeProcess(process *p){
    process *aux;
    //Anda pela lista de processos
    for (; p != NULL; p = aux){
        aux = p->prox;
        free(p);//libera o processo
    }
}
void insertProcess(process *p, process *novo){
    //anda até o final da lista encadeada
    for (; p->prox != NULL; p = p->prox);
    
    p->prox = novo;//adiciona o novo processo ao final da lista
}
void removeProcess(process *p, int pid){
    process *ant = p;//defini o anterior como processo atual auxiliar
    process *prox = p->prox;//defini o proximo da lista

    //anda pela lista
    for (process *atual = prox; atual != NULL; atual = atual->prox, ant=ant->prox){
        //entra caso ache o processo e ele seja o ultimo da lista
        if (atual->pid == pid && atual->prox == NULL){
            //remove da lista e sai do loop
            ant->prox = NULL;
            break;
        }
        else if (atual->pid == pid){//entra caso ache o processo e ele não seja o ultimo da lista
            //remove da lista e sai do loop
            ant->prox = atual->prox;
            break;
        }
    }
}
void treatsSIGUSR1(process* p){
    int status;//variavel do status do processo
   
    //anda pela lista de processos
    for(process *aux = p; aux != NULL; aux = aux->prox){
            waitpid(aux->pid, &status, WNOHANG);//verifica o status de cada processo da lista
            int s = status&255 ;//retorna o numero do sinal que matou o processo caso ele tenha morrido por sinal
            //entra se o sinal que matou o processo foi um SIGUSR1
            if(s == 10){
                //anda pela lista de processos
                for (process *aux2 = p; aux2 != NULL; aux2 = aux2->prox)
                    if(aux2->pid != aux->pid){//entra caso o processo seja diferente do processo que recebeu o sinal
                        kill(aux2->pid,SIGUSR1);//envia o sinal de SIGUSR1
                        //Espera ate que o zombi suma
                        int i=0, statusf;
                        while(i==0)
                            i=waitpid(aux2->pid,&statusf,WNOHANG);
                    }
                //manda um alerta para a main avisando da nova cepa
                kill(getpid(), SIGALRM);
                break;//retorna para a main
            }
    }
}
void psNotDie(int num){
    //Main vacinada imprime 
    write(STDERR_FILENO, "\nEstou vacinada...desista!!\n", 28);
    write(STDERR_FILENO, "psh>", 4);
}
void newCepa(int num){
    printf("You shall not pass: %d\n",num);
    printf("\n                        ,---.");
    printf("\n                       /    |");
    printf("\n                      /     |");
    printf("\n                     /      |");
    printf("\n                    /       |");
    printf("\n               ___,'        |");
    printf("\n             <  -'          :");
    printf("\n              `-.__..--'``-,_\\_");
    printf("\n                 |o/ ` :,.)_`>");
    printf("\n                 :/ `     ||/)");
    printf("\n                 (_.).__,-` |\\");
    printf("\n                 /( `.``   `| :");
    printf("\n                 \'`-.)  `  ; ;");
    printf("\n                 | `       /-<");
    printf("\n                 |     `  /   `.");
    printf("\n ,-_-..____     /|  `    :__..-'\\");
    printf("\n/,'-.__\\  ``-./ :`      ;       \\");
    printf("\n`\\ `\\  `\\  \\ :  (   `  /  ,   `. \\");
    printf("\n  \\` \\   \\   |  | `   :  :     .\\ \\");
    printf("\n   \\ `\\_  ))  :  ;     |  |      ): :");
    printf("\n  (`-.-'\\ ||  |\\ \\   ` ;  ;       | |");
    printf("\n   \\-_   `;;._   ( `  /  /_       | |");
    printf("\n    `-.-.// ,'`-._\\__/_,'         ; |");
    printf("\n       \\:: :     /     `     ,   /  |");
    printf("\n        || |    (        ,' /   /   |");
    printf("\n        ||                ,'   /    |");
    printf("\n________ Unfortunately all process died!________");
    printf("\n___ Vaccination should be a social contract!____");
    printf("\n____Cooperation was the morally right choice!___\n");
}
void pshellBackground(process* p){
    for(process* aux = p->prox; aux!=NULL; aux = aux->prox){
        if(aux->identify == 1){
            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(STDIN_FILENO, aux->pid);
            printf("\n%d\n", aux->pid);
            sleep(30);
            tcsetpgrp(STDIN_FILENO, getpid());
            break;
        }
    }
}
void typePrompt(){
    static int first_time = 1;
    if (first_time){
        const char *CLEAR_SCREEN_ANSI = " \e[1;1H\e[2J";
        write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
        first_time = 0;
    }
    printf("psh>");
}
void updadeProcessList(process* p){
    for (process *aux = p; aux != NULL; aux = aux->prox)
        if (kill(aux->pid, SIGCHLD) == -1)
            removeProcess(p, aux->pid);
}
int readCommand(char **cmd){
    char linha[1024];
    char *tok;
    int count = 0, i = 0;

    // Pega a linha digitada
    for (;;){
        int c = fgetc(stdin);
        linha[count++] = (char)c;
        if (c == '\n')
            break;
    }
    linha[count] = '\0';
    // retorna caso não haja comandos
    if (count == 1)
        exit(EXIT_SUCCESS);

    tok = strtok(linha, ";\n");
    // Divide os comandos caso seja executado mais de um
    while (tok){
        cmd[i++] = strdup(tok);
        tok = strtok(NULL, ";\n");
    }
    return i;
}
int treatmentCommand(char *cmd, char **par, int *direcionaSaida, char *nameFile){
    int i = 0;
    char *array[100];
    
    char *tok = strtok(cmd, ">");
    cmd = strdup(tok);
    tok = strtok(NULL, ">");
    if (tok)
    {
        array[0] = strdup(tok);
        tok = strtok(array[0], " ");
        array[0] = strdup(tok);
        strcpy(nameFile, array[0]);
        direcionaSaida[0] = 1;
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

int notVaccinated(int *direcionaSaida, char *commad, char **parameters, char *nameFile, process *processos){
    int status;//armazedo o retorno da função execvp
    int saida;//
    pid_t pid = fork();//gera novo processo
    
    if (pid != 0){//Main
        //entra casso a lista de processos esteja vazia
        if (processos == NULL)
            processos = inicProcess(pid, 0);//incere novo processo
        else{//entra caso já haja uma lista de processos
            process *novo = inicProcess(pid, 0);//cria novo processo
            insertProcess(processos, novo);//adiciona o processo a lista
        }
    }else{//Processo
        setpgid(0, 0);//Isola o processo    
        
        // Redirecionamento da Saida para arquivo
        if (direcionaSaida[0]){
            saida = open(nameFile, O_APPEND | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
            if (saida == -1)
                return 1;
            if (dup2(saida, fileno(stdout)) == -1){
                close(saida);
                return 2;
            }
            close(saida);
        }
        //////////////////////////
        
        status=execvp(commad, parameters);//executa o comando
        
        if(status == -1)
            return 4;
        
        exit(EXIT_SUCCESS);
    }
    return 0;
}


int Vaccinated(char **command, int qtdCommand, char **parameters, int *direcionaSaida, char *nameFile, pid_t pid_group, process *processos){
    int saida;
    pid_t pid;
    sigset_t newsigset;
    if ((sigemptyset(&newsigset) == -1) || (sigaddset(&newsigset, SIGINT) == -1) || (sigaddset(&newsigset, SIGQUIT) == -1) || (sigaddset(&newsigset, SIGTSTP) == -1))
        return 5;
    if (sigprocmask(SIG_BLOCK, &newsigset, NULL) == -1)
        return 6;
    
    for (int j = 0; j < qtdCommand; j++){
        pid = fork();
        if(pid != 0 && j==0)
            sleep(1);
        if(pid_group == 0 ) pid_group=pid;
        if (pid == 0){ // filho
            pid_t aux = getpid();
            if(j == 0 && pid_group == 0)setpgid(aux, 0);
            else setpgid(aux, pid_group);
            int qtdPar = treatmentCommand(command[j], parameters, direcionaSaida, nameFile);
            // Redirecionamento da Saida
            if (direcionaSaida[0]){
                saida = open(nameFile, O_APPEND | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
                if (saida == -1)
                    return 1;
                if (dup2(saida, fileno(stdout)) == -1){
                    close(saida);
                    return 2;
                }
                close(saida);
            }
            //////////////////////////
            int status = execvp(parameters[0], parameters);
            if (status == 1)
                return 4;
            exit(EXIT_SUCCESS);
        }
        else{
            if (processos == NULL)
                processos = inicProcess(pid, 1);
            else{
                process *novo = inicProcess(pid, 1);
                insertProcess(processos, novo);
            }
        }
    }
    return pid_group;
}