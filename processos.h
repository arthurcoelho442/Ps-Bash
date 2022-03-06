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

process* inicProcess(int pid, int idt);
void freeProcess(process *p);
void insertProcess(process *p, process *novo);
void removeProcess(process *p, int pid);
void treatsSIGURS1(process* p);
void psNotDie(int num);
void newCepa(int num);
void pshellBackground(process* p);
void typePrompt();
void updadeProcessList(process* p);

int readCommand(char **cmd);
int treatmentCommand(char* cmd, char **par, int* direcionaSaida, char *nameFile);
int notVaccinated(int* direcionaSaida, char* commad, char** parameters, char* nameFile, process* processos);
int Vaccinated(char **command, int qtdCommand, char **parameters, int *direcionaSaida, char *nameFile, pid_t pid_group, process *processos);

#endif