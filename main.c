#include <stdio.h>
#include <stdlib.h>
#include "processos.h"

int main(int argc, char **argv){
    signal(SIGINT, psNotDie);
    signal(SIGQUIT, psNotDie);
    signal(SIGTSTP, psNotDie);
    signal(SIGUSR1, psNotDie);
    signal(SIGALRM, newCepa);

    char *command[100], *parameters[100];
    int qtdCommand, saveOut = dup(fileno(stdout));
    pid_t pid_group;
    char nameFile[100];
    int direcionaSaida[1];
    direcionaSaida[0] = 0;
    process* processos = inicProcess(0,0);
    
    while(1){
        typePrompt();
        qtdCommand = readCommand(command);

        treatsSIGUSR1(processos);
        if (strcmp(command[0], "term") == 0 && qtdCommand == 1){
            for(process* aux = processos->prox; aux!=NULL; aux = aux->prox)
                kill(aux->pid, SIGKILL);
            freeProcess(processos);
            exit(EXIT_SUCCESS);
        }

        if(strcmp(command[0], "fg") == 0 &&  qtdCommand == 1){
            pshellBackground(processos);
            qtdCommand = 0;
        }

        if (qtdCommand == 1){ // não vacinado
            int qtdPar=treatmentCommand(command[0], parameters, direcionaSaida, nameFile);
            switch(notVaccinated(direcionaSaida, parameters[0], parameters, nameFile, processos)){
                case 1:
                    perror("Não foi possivel abrir nem criar o arquivo\n");
                    break;
                case 2:
                    perror("Não foi possivel redirecionar a saida\n");
                    break;
                case 3:
                    perror("Não foi possivel redirecionar a saida padrao\n");
                    break;
                case 4:
                    printf("Erro, comando ou parametro errado\n");
                    break;
            }
            for (int j = 0; j < qtdPar; j++)
                free(parameters[j]);
            if (direcionaSaida[0]){
                dup2(saveOut, 1);
                close(saveOut);
                direcionaSaida[0] = 0;
            }
        }
        else if (qtdCommand > 1){// vacinados
            int status;
            pid_t aux;

            for (process *verifica = processos->prox->pid; verifica != NULL; verifica = verifica->prox){
                if (waitpid(pid_group, &status, WNOHANG) == 0 && verifica->identify == 1){
                    pid_group = verifica->pid;
                    break;
                } else
                    pid_group = 0;
            }
            
            aux = Vaccinated(command, qtdCommand, parameters, direcionaSaida, nameFile, pid_group, processos);
            switch (aux){
            case 1:
                perror("Não foi possivel abrir nem criar o arquivo\n");
                break;
            case 2:
                perror("Não foi possivel redirecionar a saida\n");
                break;
            case 3:
                perror("Não foi possivel redirecionar a saida padrao\n");
                break;
            case 4:
                perror("Erro, comando ou parametro errado\n");
                break;
            case 5:
                perror("Não foi possivel instalar os sinais\n");
                break;
            case 6:
                perror("Não foi possivel bloquear os sinais instalados\n");
                break;
            default:
                pid_group=aux;
                break;
            }
            if (direcionaSaida[0]){
                dup2(saveOut, 1);
                close(saveOut);
                direcionaSaida[0] = 0;
            }
        }
        updadeProcessList(processos);
        sleep(1);
    }
    exit(EXIT_SUCCESS);
}