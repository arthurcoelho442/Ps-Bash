#include <stdio.h>
#include <stdlib.h>
#include "processos.h"

int main(int argc, char **argv){
    /* Define Handler para os sinais */
    signal(SIGINT, psNotDie); 
    signal(SIGQUIT, psNotDie);
    signal(SIGTSTP, psNotDie);
    signal(SIGUSR1, psNotDie);
    signal(SIGALRM, newCepa);

    //Declara as
    char *command[100], *parameters[100];
    int qtdCommand, saveOut = dup(fileno(stdout));
    pid_t pid_group=0;
    char nameFile[100];
    int direcionaSaida[1];  
    direcionaSaida[0] = 0;  
    process* processos = inicProcess(0,0);
    
    while(1){
        /* Inicia linha de comando 'psh>' */
        typePrompt();

        /* Le os comandos digitados no teclado e retorna a qtd de comandos digitados */
        qtdCommand = readCommand(command);

        /* Propaga o sinal do SIGURS1  entre os processos*/
        treatsSIGUSR1(processos);

        /* Mata todos os processos e termina o programa */
        if (strcmp(command[0], "term") == 0 && qtdCommand == 1){
            for(process* aux = processos->prox; aux!=NULL; aux = aux->prox)
                kill(aux->pid, SIGKILL);    //Mata processo de PID == aux->pid
            freeProcess(processos); //Libera lista de processos 
            exit(EXIT_SUCCESS);
        }
        /* Coloca temporariamente o grupo de vacinados em foreground e psh em Background, por 30 segundos */
        if(strcmp(command[0], "fg") == 0 &&  qtdCommand == 1){
            pshellBackground(processos);
            qtdCommand = 0; //Seta quantidade de comando para 0;
        }

        if (qtdCommand == 1){ // não vacinado
            //trata o comando digitado
            int qtdPar=treatmentCommand(command[0], parameters, direcionaSaida, nameFile);
            //Executa o não vacinado e imprime na tela caso haja algum erro
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
            //Retorna o direcionamento para o padrão
            for (int j = 0; j < qtdPar; j++)
                free(parameters[j]);
            if (direcionaSaida[0]){
                dup2(saveOut, 1);
                close(saveOut);
                direcionaSaida[0] = 0;
            }
            /////////////////////////////////////////
        }
        else if (qtdCommand > 1){// vacinados
            pid_t aux;
            
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
            //Retorna o direcionamento para o padão
            if (direcionaSaida[0]){
                dup2(saveOut, 1);
                close(saveOut);
                direcionaSaida[0] = 0;
            }
            ///////////////////////////////////////
        }
        updadeProcessList(processos);//Verifica se algum processo já moreu e atualiza a lista
        sleep(1);//espera 1 segundo para printar ps> na saida padrão
    }
    exit(EXIT_SUCCESS);
}