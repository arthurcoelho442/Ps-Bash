#ifndef PROCESSOS_H
#define PROCESSOS

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

struct process{
    int pid;
    int identify;
    struct process* prox;
};

typedef struct process process;

/*
* Função inicProcess: Inicia um processo.
* Inputs: Pid do processo, id vacidano/nãoVacinado.
* Outputs: Ponteiro para processo.
* Pre-Condicao: Nenhuma.
* Pos-Condicao: Retornar um ponteiro não Nulo.
*/
process* inicProcess(int pid, int idt);
/*
* Função freeProcess: Libera todos os processos.
* Inputs: Ponteiro para o inicio da lista.
* Outputs: Nenhum.
* Pre-Condicao: Nenhuma.
* Pos-Condicao: Lista é liberada corretamente.
*/
void freeProcess(process *p);
/*
* Função InsertProcess: Insere um processo no final da lista encadeada.
* Inputs: Ponteiro para o inicio da lista, processo a ser adicionado.
* Outputs: Nenhum.
* Pre-Condicao: Lista existir.
* Pos-Condicao: Ponteiro adicionado na lista.
*/
void insertProcess(process *p, process *novo);
/*
* Função RemoveProcess: Remove processo da lista.
* Inputs: Ponteiro para o inicio da lista, pid do processo a ser removido.
* Outputs: Nenhum.
* Pre-Condicao: Lista existir.
* Pos-Condicao: Processo é removido da lista.
*/
void removeProcess(process *p, int pid);
/*
* Função treatsSIGUSR1: Propaga o sinal do SIGURS1.
* Inputs: Ponteiro para o inicio da lista.
* Outputs: Nenhum.
* Pre-Condicao: Nenhum.
* Pos-Condicao: Todos os processos recebem SIGUSR1.
*/
void treatsSIGUSR1(process* p);
/*
* Função psNotDie: Impede que a main execute o sinal.
* Inputs: numero do sinal.
* Outputs: Nenhum.
* Pre-Condicao: Nenhum.
* Pos-Condicao: Nenhum.
*/
void psNotDie(int num);
/*
* Função newCepa: Main descobre sobre a nova sepa do virus.
* Inputs: numero do sinal.
* Outputs: Nenhum.
* Pre-Condicao: Nenhum.
* Pos-Condicao: Nenhum.
*/
void newCepa(int num);
/*
* Função pshellBackground: Colcoca a Ps em backgroud e os processos vacinados em foreground por 30s.
* Inputs: Ponteiro para o inicio da lista.
* Outputs: Nenhum.
* Pre-Condicao: Nenhum.
* Pos-Condicao: Nenhum.
*/
void pshellBackground(process* p);
/*
* Função typePromptinicia : Inicia linha de comando.
* Inputs: Nenhum.
* Outputs: Nenhum.
* Pre-Condicao: Nenhum.
* Pos-Condicao: Nenhum.
*/
void typePrompt();
/*
* Função updadeProcessList : Verifica se a processos mortos e os remove da lista.
* Inputs: Ponteiro para o inicio da lista.
* Outputs: Nenhum.
* Pre-Condicao: Nenhum.
* Pos-Condicao: Nenhum.
*/
void updadeProcessList(process* p);
/*
* Função readCommand : Le o comando digitado no teclado.
* Inputs: Ponteiro para o comando.
* Outputs: retorna a quantidade de comando lidos.
* Pre-Condicao: Nenhum.
* Pos-Condicao: Nenhum.
*/
int readCommand(char **cmd);
/*
* Função treatmentCommand : Trata o comando em parametros e indica se existe direcionamento de saida.
* Inputs: comando, parametros, indicativo de direcionamento, nome do arquivo para direcionamento.
* Outputs: Retorna a quantidade de parametros do comando.
* Pre-Condicao: Nenhum.
* Pos-Condicao: Nenhum.
*/
int treatmentCommand(char* cmd, char **par, int* direcionaSaida, char *nameFile);
/*
* Função notVaccinated : Cria um processo não vacinado e adiciona-o a lista de processos.
* Inputs: indicativo de direcionamento, comando, parametros, nome do arquivo para direcionamento e a lista de processos.
* Outputs: retorna 0 para executado com exito e diferente de 0 para o numero do erro.
* Pre-Condicao: Nenhum.
* Pos-Condicao: Nenhum.
*/
int notVaccinated(int* direcionaSaida, char* commad, char** parameters, char* nameFile, process* processos);
/*
* Função Vaccinated : Cria todos os processo vacinado e adiciona-os a lista de processos.
* Inputs: indicativo de direcionamento, comando, quantidade de comandos, parametros, nome do arquivo para direcionamento, pid do grupo caso já exista um e a lista de processos.
* Outputs: retorna 0 para executado com exito e diferente de 0 para o numero do erro, retorna o pid do grupo.
* Pre-Condicao: Nenhum.
* Pos-Condicao: Nenhum.
*/
int Vaccinated(char **command, int qtdCommand, char **parameters, int *direcionaSaida, char *nameFile, pid_t pid_group, process *processos);

#endif