#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "cliente2014.h"
#include <pthread.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>


int cria_socket_local(void)
{
	int socket_local;		/* Socket usado na comunicacão */

	socket_local = socket( PF_INET, SOCK_DGRAM, 0);
	if (socket_local < 0) {
		perror("socket");
		return;
	}
	return socket_local;
}

struct sockaddr_in cria_endereco_destino(char *destino, int porta_destino)
{
	struct sockaddr_in servidor; 	/* Endereço do servidor incluindo ip e porta */
	struct hostent *dest_internet;	/* Endereço destino em formato próprio */
	struct in_addr dest_ip;			/* Endereço destino em formato ip numérico */

	if (inet_aton ( destino, &dest_ip ))
		dest_internet = gethostbyaddr((char *)&dest_ip, sizeof(dest_ip), AF_INET);
	else
		dest_internet = gethostbyname(destino);

	if (dest_internet == NULL) {
		fprintf(stderr,"Endereço de rede inválido\n");
		exit(FALHA);
	}

	memset((char *) &servidor, 0, sizeof(servidor));
	memcpy(&servidor.sin_addr, dest_internet->h_addr_list[0], sizeof(servidor.sin_addr));
	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(porta_destino);

	return servidor;
}

void envia_mensagem(int socket_local, struct sockaddr_in endereco_destino, char *mensagem)
{
	/* Envia msg ao servidor */

	if (sendto(socket_local, mensagem, strlen(mensagem)+1, 0, (struct sockaddr *) &endereco_destino, sizeof(endereco_destino)) < 0 )
	{ 
		perror("sendto");
		return;
	}
}

int recebe_mensagem(int socket_local, char *buffer, int TAM_BUFFER)
{
	int bytes_recebidos;		/* Número de bytes recebidos */

	/* Espera pela msg de resposta do servidor */
	bytes_recebidos = recvfrom(socket_local, buffer, TAM_BUFFER, 0, NULL, 0);
	if (bytes_recebidos < 0)
	{
		perror("recvfrom");
	}

	return bytes_recebidos;
}

//função para leitura e tratamento da mensagem recebida pelo programa da planta
float leituraTratamento(char *stringLeitura)
{
	char stringAux[TAMANHO_STRING];
	float valorRetorno;

	envia_mensagem(socket_global, endereco_global, stringLeitura);
	nrec = recebe_mensagem(socket_global, stringAux, TAMANHO_STRING);

	//coloca-se um "\0" no final para indicar fim do vetor
	stringAux[TAMANHO_STRING-1] = '\0';

	//converte o string recebido pela planta para um float
	valorRetorno = atof(&stringAux[3]);

	return valorRetorno; //retorna um valor float
}

//função que realiza a atuação de temperatura Q
void atuaTemp (float qTemp)
{
	char stringAux[TAMANHO_STRING];

	//escreve na variavel stringAux o comando e o valor de atuação de Q (qOn ou qOff).
	sprintf(stringAux,"aq-%f",qTemp);
	//printf("string sendo enviada = %s\n",stringAux);
	envia_mensagem(socket_global, endereco_global, stringAux);
	nrec = recebe_mensagem(socket_global, stringAux, TAMANHO_STRING);
}

//função que realiza a atuação da válvula de entrada Ni
void atuaNi (float niAltura)
{
	char stringAux1[TAMANHO_STRING];

	sprintf(stringAux1,"ani%f",niAltura);
	envia_mensagem(socket_global, endereco_global, stringAux1);
	nrec = recebe_mensagem(socket_global, stringAux1, TAMANHO_STRING);	
}

//função que escreve na tela os valores das variáveis de 1 em 1 segundo
void escreveNaTela(void)
{

	system("clear");
	printf("\n=================MONITORAMENTO DAS VARIAVEIS============\n");
	printf("Ta = %.3f \n",Ta);
	printf("T = %.3f  ----- Tref = %.3f\n",T,Tref);
	printf("Ti = %.2f \n",Ti);
	printf("No = %.2f \n",No);
	printf("H = %.3f  ----- Href = %.3f\n",H,Href);
	printf("===================VALORES DE ATUACAO===================\n");
	printf("Q = %.3f\n",Q);
	printf("Ni = %.3f\n",Ni);
	printf("Status do Alarme = %s\n",flagAlarme );
	printf("========================================================\n");
	printf("Pressione enter para entrar com um valor de referencia:\n");;
}

//função que escreve na tela os valores bem como o Alarme para nível
void escreveAlarme(void)
{
	system("clear");
	printf("\n=================MONITORAMENTO DAS VARIAVEIS============\n");
	printf("Ta = %.3f \n",Ta);
	printf("T = %.3f  ----- Tref = %.3f\n",T,Tref);
	printf("Ti = %.2f \n",Ti);
	printf("No = %.2f \n",No);
	printf("H = %.3f  ----- Href = %.3f\n",H,Href);
	printf("===================VALORES DE ATUACAO===================\n");
	printf("Q = %.3f\n",Q);
	printf("Ni = %.3f\n",Ni);
	printf("Status do Alarme = LIGADO!!!!!!!!\n" );
	printf("====================================================\n");
	printf("Pressione enter para entrar com um valor de referencia:\n");
}

//Imprime no arquivo os logs do programa
void escreveArquivo()
{
		fprintf(arquivo,"=================MONITORAMENTO DAS VARIAVEIS=========\n");
		fprintf(arquivo,"Ta = %.3f \n",Ta);
		fprintf(arquivo,"T = %.3f  ----- Tref = %.3f\n",T,Tref);
		fprintf(arquivo,"Ti = %.3f \n",Ti);
		fprintf(arquivo,"No = %.3f \n",No);
		fprintf(arquivo,"H = %.3f  ----- Href = %.3f\n",H,Href);
		fprintf(arquivo,"===================Valores da atuaçao===================\n");
		fprintf(arquivo,"Q = %.3f\n",Q);
		fprintf(arquivo,"Ni = %.3f\n",Ni);
		fprintf(arquivo,"status do alarme: %s\n",flagAlarme);
		fprintf(arquivo,"========================================================\n");
		fflush(arquivo);
}