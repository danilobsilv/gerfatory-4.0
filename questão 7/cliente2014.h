#ifndef CLIENTE2014_H
#define CLIENTE2014_H

#define FALHA 1
#define TAM_MEU_BUFFER	1000
#define TAMANHO_STRING 10000
#define leTempA "sta0"
#define leTemp "st-0"
#define leTempI "sti0"
#define leNo "sno0"
#define leAltura "sh-0"
#define qOn 100000.0
#define qOff 0.0
#define niOn 100.00
#define niOff 0.0
#define NSEC_PER_SEC    (1000000000) /* Número de nseg em 1 segundo */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//variáveis globais
int socket_global;
int nrec;
struct sockaddr_in endereco_global;
FILE *arquivo;
float Ta, T, Ti, No, H, Ni, Q, Href, Tref;
char flagAlarme[10];
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_cond_t dormeAlarme;


//funções da biblioteca
int cria_socket_local();
struct sockaddr_in cria_endereco_destino(char *destino, int porta_destino);
void envia_mensagem(int socket_local, struct sockaddr_in endereco_destino, char *mensagem);
int recebe_mensagem(int socket_local, char *buffer, int TAM_BUFFER);
float leituraTratamento(char *stringLeitura);
void atuaTemp (float qTemp);
void atuaNi (float niAltura);
void escreveNaTela();
void escreveArquivo();
void escreveAlarme();

#endif