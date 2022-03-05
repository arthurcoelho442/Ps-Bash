#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

typedef struct process
{
    int pid;
    int status;
    int complete;
    int identify;
    struct process *prox;
} process;
process *inicProcess(int pid, int identificador)
{
    process *p = (process *)malloc(sizeof(process));
    p->pid = pid;
    p->complete = 0;
    p->prox = NULL;
    p->identify = identificador;
    return p;
}
void freeProcess(process *p)
{
    process *aux;
    for (; p != NULL; p = aux)
    {
        aux = p->prox;
        free(p);
    }
}

void inseriProcess(process *p, process *novo)
{
    for (; p->prox != NULL; p = p->prox)
        ;
    p->prox = novo;
}
void imprimeProcess(process *p)
{
    for (process *aux = p; aux != NULL; aux = aux->prox)
        printf("Não vacinado pid: %d\n", aux->pid);
}
void removeProcess(process *p, int pid)
{
    process *ant = p;
    process *prox = p->prox;
    process *aux2;

    for (process *atual = prox; atual != NULL; atual = atual->prox, ant=ant->prox)
    {

        if (atual->pid == pid && atual->prox == NULL)
        {
            ant->prox = NULL;
            free(atual);
        }
        else if (atual->pid == pid)
        {
            ant->prox = atual->prox;
            free(atual);
        }
    }
}
void trata_SIGURS1(process* p){
    int status;
    for(process *aux = p; aux != NULL; aux = aux->prox){
            waitpid(aux->pid, &status, WNOHANG);
            int s = status&255 ;
            if(s == 10){
                for (process *aux2 = p; aux2 != NULL; aux2 = aux2->prox)
                    if(aux2->pid != aux->pid)
                        kill(aux2->pid,SIGUSR1);
                kill(getpid(), SIGALRM);
                break;
            }
    }
}
void type_prompt();
int read_command(char **cmd);
void processInfo(char *msg);
int treatment_command(char *cmd, char **par, int *direcionaSaida, char *nameFile);
int naoVacinado(int *direcionaSaida, char *commad, char **parameters, char *nameFile, process *processos);
int vacinados(char **command, int qtdCommand, char **parameters, int *direcionaSaida, char *nameFile, pid_t pid_group, process *processos);
int grupo_vacinados(int flag);
void psNaoMorrer(int num)
{
    write(STDERR_FILENO, "\nEstou vacinada...desista!!\n", 28);
}
void novaCepa()
{
    char *text = "\n                       ,---.\n                       /    |\n                      /     |\n                     /      |\n                    /       |\n               ___,'        |\n             <  -'          :\n              `-.__..--'``-,_\'_\n                 |o/ ` :,.)_`>\n                 :/ `     ||/)\n                 (_.).__,-` |\\n                 /( `.``   `| :\n                 \'`-.)  `  ; ;\n                 | `       /-<\n                 |     `  /   `.\n ,-_-..____     /|  `    :__..-'\\n/,'-.__\\  ``-./ :`      ;       \\n`\' `\'  `\\  \' :  (   `  /  ,   `. \\n  \'` \'   \\   |  | `   :  :     .\' \\n   \' `\'_  ))  :  ;     |  |      ): :\n  (`-.-'\' ||  |\' \'   ` ;  ;       | |\n   \'-_   `;;._   ( `  /  /_       | |\n    `-.-.// ,'`-._\'__/_,'         ; |\n       \':: :     /     `     ,   /  |\n        || |    (        ,' /   /   |\n        ||                ,'   /    |\n________ Unfortunately all process died!________\n___ Vaccination should be a social contract!____\n____Cooperation was the morally right choice!___\n";
    //(SIGABRT, SIG_DFL);
}
int main(int argc, char **argv)
{
    signal(SIGINT, psNaoMorrer);
    signal(SIGQUIT, psNaoMorrer);
    signal(SIGTSTP, psNaoMorrer);
    signal(SIGUSR1, novaCepa);
    signal(SIGALRM, novaCepa);

    char cmd[100], *command[100], *parameters[100];
    int qtdCommand;
    pid_t pid, pid_group;
    int count = 0;
    int flag = 0;
    int saida, saveOut;
    char nameFile[100];
    int direcionaSaida[1];
    direcionaSaida[0] = 0;
    process *processos = inicProcess(0,0);

    while (1)
    {
        type_prompt();
        qtdCommand = read_command(command);

        if (strcmp(command[0], "term") == 0 && qtdCommand == 1)
            exit(EXIT_SUCCESS);
        if (qtdCommand == 1)
        { // não vacinado
            int qtdPar = treatment_command(command[0], parameters, direcionaSaida, nameFile);
            switch (naoVacinado(direcionaSaida, parameters[0], parameters, nameFile, processos))
            {
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
                perror("Erro, comando ou parametro errado");
                break;
            }
            for (int j = 0; j < qtdPar; j++)
                free(parameters[j]);
            imprimeProcess(processos);
        }
        else if (qtdCommand > 1)
        { // vacinados
            int status;
            pid_t retorno_group = 0, aux;
            if (flag == 1)
                retorno_group = waitpid(pid_group, &status, WNOHANG);
            if (retorno_group != 0)
                flag = 0;

            aux = grupo_vacinados(flag);
            if (aux > 0)
                pid_group = aux;
            int aux1 = vacinados(command, qtdCommand, parameters, direcionaSaida, nameFile, pid_group, processos);
            switch (aux1)
            {
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
                perror("Erro, comando ou parametro errado");
                break;
            case 5:
                perror("Não foi possivel instalar os sinais");
                break;
            case 6:
                perror("Não foi possivel bloquear os sinais instalados");
                break;
            default:
                flag = 1;
                break;
            }
        }
        int count = 0;
        int statusf;
        printf("\n\nantes do fopr: %d\n\n", count);

        trata_SIGURS1(processos);

        for (process *aux = processos; aux != NULL; aux = aux->prox)
        {
            printf("\n\nsinal:%d count:%d//pid:%d//indentificador:%d\n\n ",waitpid(aux->pid, &statusf, WNOHANG), count, aux->pid, aux->identify);
            if (aux->identify == 1)
                count += 1;
            
            if (kill(aux->pid, SIGCHLD) == -1)
            {
                if (aux->identify == 1) count-=1;
                removeProcess(processos, aux->pid);
            }
        }
        printf("\n\ndepois do fopr: %d\n\n", count);
        if (count == 0 && processos != NULL)
        {
            kill(pid_group, SIGKILL);
        }
    }
    exit(EXIT_SUCCESS);
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

int read_command(char **cmd)
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
    linha[count] = '\0';
    // retorna caso não haja comandos
    if (count == 1)
        exit(EXIT_SUCCESS);
    ;

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

int treatment_command(char *cmd, char **par, int *direcionaSaida, char *nameFile)
{
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

int naoVacinado(int *direcionaSaida, char *commad, char **parameters, char *nameFile, process *processos)
{
    int status;
    int saida, saveOut;
    pid_t pid = fork();
    if (pid != 0)
    {
        processInfo("Main");
        if (processos == NULL)
        {
            processos = inicProcess(pid, 0);
        }
        else
        {
            process *novo = inicProcess(pid, 0);
            inseriProcess(processos, novo);
        }
        if (direcionaSaida[0])
        {
            if (dup2(saveOut, 1) == -1)
            {
                close(saveOut);
                return 3;
            }
            close(saveOut);
            direcionaSaida[0] = 0;
        }
        return 0;
    }
    else
    {
        // Redirecionamento da Saida
        if (direcionaSaida[0])
        {
            saida = open(nameFile, O_APPEND | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
            if (saida == -1)
                return 1;

            saveOut = dup(fileno(stdout));

            if (dup2(saida, 1) == -1)
            {
                close(saida);
                return 2;
            }
            close(saida);
        }
        //////////////////////////
        setpgid(0, 0);
        processInfo("filho");

        status = execvp(commad, parameters);

        if (status == -1)
            return 4;
        exit(EXIT_SUCCESS);
        ;
    }
}

int vacinados(char **command, int qtdCommand, char **parameters, int *direcionaSaida, char *nameFile, pid_t pid_group, process *processos)
{
    processInfo("Deus");
    int saida, saveOut;
    pid_t pid;
    sigset_t newsigset;
    if ((sigemptyset(&newsigset) == -1) || (sigaddset(&newsigset, SIGINT) == -1) || (sigaddset(&newsigset, SIGQUIT) == -1) || (sigaddset(&newsigset, SIGTSTP) == -1))
        return 5;
    if (sigprocmask(SIG_BLOCK, &newsigset, NULL) == -1)
        return 6;
    int j;
    for (j = 0; j < qtdCommand; j++)
    {
        pid_t test;
        int statusasd;
        test = waitpid(pid_group, &statusasd, WNOHANG);
        pid = fork();
        if (pid == 0)
        { // filho
            pid_t aux = getpid();
            int ak = setpgid(aux, pid_group);

            int qtdPar = treatment_command(command[j], parameters, direcionaSaida, nameFile);
            // Redirecionamento da Saida
            /*if (direcionaSaida[0])
            {
                saida = open(nameFile, O_APPEND | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
                if (saida == -1)
                    return 1;
                saveOut = dup(fileno(stdout));
                if (dup2(saida, fileno(stdout)) == -1)
                    return 2;
                close(saida);
            }*/
            //////////////////////////
            processInfo("filho");
            int status = execvp(parameters[0], parameters);
            /*for (int k = 0; k < qtdPar; k++)
                free(parameters[k]);*/
            if (status == 1)
                return 4;

            if (direcionaSaida[0])
            {
                dup2(saveOut, fileno(stdout));
                close(saveOut);
                direcionaSaida[0] = 0;
            }
            exit(EXIT_SUCCESS);
        }
        else
        {

            if (processos == NULL)
            {
            
                processos = inicProcess(pid, 1);
            }
            else
            {
                
                process *novo = inicProcess(pid, 1);
                inseriProcess(processos, novo);
            }
        }
    }
    return pid_group;
}

int grupo_vacinados(int flag)
{
    if (flag == 0)
    {
        pid_t pid;
        pid = fork();
        if (pid != 0)
        {
            processInfo("Deus");
        }
        else
        {
            setpgid(0, 0);
            processInfo("Pai");
            while (1)
            {
            }
            exit(EXIT_SUCCESS);
        }
        sleep(1); //GAMBIARRA RESOLVER
        return pid;
    }
    else
        return 0;
}