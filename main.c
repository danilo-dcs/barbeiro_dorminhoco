
/* Grupo:
Danilo de Castro - 2017011925,
Pedro Fernandes de Oliveira - 2017016449,
Lucas Daves - 2017012397
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_CADEIRAS 10        //Numero de lugares na barbearia
#define NUM_BARB 3           //Numero de barbeiros
#define MAX_CLI 50          //Maximo de clientes

sem_t clientes;                 //Semaforo
sem_t barbeiros;                   //Semaforo
sem_t mutex;                     //Semaforo

//VARIÁVEIS COMPARTILHADAS
int cadeirasLivres = MAX_CADEIRAS;   //Contador de lugares livres na barbearia
int identificadorAssento[MAX_CADEIRAS];         // troca de referencia entre cada barbeiro e cliente através de "ID's"
int proximoAssentoLivre = 0;                  //indice do proximo assento livre
int proximoCorte = 0;                  //indice do proximo cliente candidato ao corte
static int cont = 0;                 //contador do numero dos clientes
int desistencias = 0;


//FUNÇÕES
void threadBarbeiro(void *tmp);         //Funcao Thread barbeiro
void threadCliente(void *tmp);       //Funcao Thread do cliente
int tempoCliente();			// tempo de atraso para a entrada dos clientes
int tempoCorte();                          // tempo de atraso do corte


int main()
{
    	pthread_t barbeiro[NUM_BARB],cliente[MAX_CLI]; //declaração das threads
    	int i,status=0;
    	//inicialização do semáforo
    	sem_init(&clientes,0,0);
    	sem_init(&barbeiros,0,0);
    	sem_init(&mutex,0,1);
    	//inicialização da thread dos barbeiros

    	printf("!!A BARBEARIA ESTA ABERTA!!\n");
	for(i=0;i<NUM_BARB;i++)                     //Criação dos 3 barbeiros
    	{
	       	status=pthread_create(&barbeiro[i],NULL,(void *)threadBarbeiro,(void*)&i);
	       	sleep(1);
	       	if(status!=0)
		  	perror("Sem barbeiros!!\n");
	}

	//inicialização das threads dos clientes
	for(i=0;i<MAX_CLI;i++)                     //Criação dos clientes
	{
		status=pthread_create(&cliente[i],NULL,(void *)threadCliente,(void*)&i);
		sleep(tempoCliente());     // pausa a chegada de clientes em x segundos
	       	if(status!=0)
			perror("Sem Clientes!!!\n");
    	}
	for(i=0;i<MAX_CLI;i++)      	// aguardando todos os clientes da fila serem atendidos
		pthread_join(cliente[i],NULL);

	printf("\n!!FIM DO ESPEDIENTE!!\n");
	printf("TOTAL DE DESISTENCIAS:   %d \n",desistencias);
	exit(EXIT_SUCCESS);  //Saida do loop infinito
}

int tempoCliente()     //Geração de número aleatório entre 1 e 6
{
     	int x = rand() % 6 +1; // colocar 6 no lugar do 2
	return x;
}

int tempoCorte()     //Geração de número aleatório entre 3 e 6
{
     	int x = rand() % 4 +3;
	return x;
}

void threadCliente(void *tmp)  /*Ação dos Clientes*/
{
	int meuLugar, Barb;
    	sem_wait(&mutex);  //Lock mutex para proteger lugares sentados
    	cont++;           //Chegada de um cliente
    	printf("Cliente -%d entrou na barbearia. ",cont);

    	if(cadeirasLivres > 0) // Caso tenha cadeiras Livres
    	{
		--cadeirasLivres;           //O cliente ocupou um lugar na barbearia
		printf("Cliente -%d sentou na sala de espera.\n",cont);
		proximoAssentoLivre = (++proximoAssentoLivre) % MAX_CADEIRAS;  //Escolhe um lugar para sentar
		meuLugar = proximoAssentoLivre;
		identificadorAssento[meuLugar] = cont;
		sem_post(&mutex);                  		//seta o mutex de assento ocupado
		sem_post(&barbeiros);                		//Acorda um barbeiro
		sem_wait(&clientes);              		//Entra na fila de clientes na espera
		sem_wait(&mutex);                  		//bloqueia o mutex para proteger lugares ocupados
		Barb = identificadorAssento[meuLugar];    	//conexão entre o Barbeiro e o cliente, o barbeiro atribui o ID do cliente para ele
		cadeirasLivres++;             			//levanta e vai para a cadeira do barbeiro
		sem_post(&mutex);                        	//seta o mutex de assento ocupado
		        					//clientes tendo o seu cabelo cortado pelo barbeiro 'B
	}
	else // Caso não tenha cadeiras Livres
	{
	       sem_post(&mutex);  //libera o mutex e o cliente vai embora sem cortar cabelo
	       printf("Cliente -%d Saiu.\n",cont);
		desistencias++;
	}
	pthread_exit(0);
}

void threadBarbeiro(void *tmp)        //Ação dos Barbeiros
{
	    int indice = *(int *)(tmp);
	    int meuProximo, Cli;

	    while(1)            				//Laço Infinito
	    {
		sem_wait(&mutex);		//bloqueando o mutex para verificar se não há cadeiras
		if(cadeirasLivres == MAX_CADEIRAS)  	// se não houver clientes
			printf("Barbeiro-%d Foi dormir.\n",indice+1);
		sem_post(&mutex);

		sem_wait(&barbeiros);          				//Entra na fila de barbeiros dormindo
		sem_wait(&mutex);          				//bloqueia o mutex para proteger os lugares
		proximoCorte = (++proximoCorte) % MAX_CADEIRAS;  	//seleciona um novo cliente
		meuProximo = proximoCorte;
		Cli = identificadorAssento[meuProximo];                  	//pega o "ID" do cliente selecionado
		identificadorAssento[meuProximo] = pthread_self();     	//atribui o seu ID para o cliente, através do vetor de ID's
		sem_post(&mutex);
		sem_post(&clientes);         				//Chama o cliente selecionado
		        						//O Barbeiro está cortando o cabelo do cliente 'C'
		printf("Barbeiro -%d está acordado e cortando o cabelo do cliente-%d.\n",indice+1,Cli);
		sleep(tempoCorte());
		printf("Barbeiro -%d Acabou o corte do cliente-%d.\n ",indice+1, Cli);
	    }
}

