#include <stdio.h>
#include <stdlib.h>
#include "list_int.h"
#include <pthread.h>
#include "timer.h"

#define QTDE_OPS 300 //quantidade de operacoes sobre a lista (insercao, remocao, consulta)
#define QTDE_INI 100 //quantidade de insercoes iniciais na lista
#define MAX_VALUE 100 //valor maximo a ser inserido

//lista compartilhada iniciada 
struct list_node_s* head_p = NULL; 
//qtde de threads no programa
int nthreads;
int leitores = 0;
int escrevendo = 0;
int querendoEscrever = 0;

//exclusão mutua e sincronização condicional
pthread_mutex_t mutex;
pthread_cond_t condLeitura, condEscrita;


void EntraLeitura(long int id){
   pthread_mutex_lock(&mutex);
   while(escrevendo || querendoEscrever){
      printf("T[%ld] Entrou na fila para ler pois tem alguém escrevendo ou na fila para escrever\n", id);
      pthread_cond_wait(&condLeitura, &mutex);
   }
   printf("T[%ld] Iniciando leitura\n", id);
   leitores++;
   pthread_mutex_unlock(&mutex);
}

void SaiLeitura(long int id){
   pthread_mutex_lock(&mutex);
   printf("T[%ld] Finalizando leitura\n", id);
   leitores--;
   if(!leitores){
      pthread_cond_signal(&condEscrita); 
   }
   pthread_mutex_unlock(&mutex);
}

void EntraEscrita(long int id){
   pthread_mutex_lock(&mutex);
   querendoEscrever++;
   while(leitores || escrevendo){ 
      printf("T[%ld] Entrou na fila para escrever pois tem alguém lendo ou já escrevendo\n", id);
      pthread_cond_wait(&condEscrita, &mutex);
   }
   querendoEscrever--;
   escrevendo = 1;
   printf("T[%ld] Indo escrever agora\n", id);
   pthread_mutex_unlock(&mutex);
}

void SaiEscrita(long int id){
   pthread_mutex_lock(&mutex);
   escrevendo = 0;
   if(querendoEscrever){
      printf("T[%ld] Escrita finalizada. Liberando o próximo para escrita\n", id);
      pthread_cond_signal(&condEscrita); // libero a escrita para o proximo da fila APENAS. Não faz sentido liberar todos visto que só ocorre 1 leitura por vez.
   }
   else{
      printf("T[%ld] Escrita finalizada. Liberando leitura pois a fila de escrita está vazia\n", id);
      pthread_cond_broadcast(&condLeitura); // posso liberar todos porque as leituras podem ocorrer simultaneamente
   }
   pthread_mutex_unlock(&mutex);
}

//tarefa das threads
void* tarefa(void* arg) {
   long int id = (long int) arg;
   int op;
   int in, out, read; 
   in=out=read = 0; 

   //realiza operacoes de consulta (50%), insercao (25%) e remocao (25%)
   for(long int i=id; i<QTDE_OPS; i+=nthreads) {
      op = rand() % 100;
      if(op<50) {
         EntraLeitura(id);
         Member(i%MAX_VALUE, head_p);   /* Ignore return value */
         SaiLeitura(id);
         read++;
      } else if(50<=op && op<75) {
         EntraEscrita(id);
         Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
         SaiEscrita(id);
         in++;
      } else if(op>=75) {
         EntraEscrita(id);
         Delete(i%MAX_VALUE, &head_p);  /* Ignore return value */
         SaiEscrita(id);
         out++;
      }
   }
   //registra a qtde de operacoes realizadas por tipo
   printf("Thread %ld: in=%d out=%d read=%d\n", id, in, out, read);
   pthread_exit(NULL);
}

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   pthread_t *tid;
   double ini, fim, delta;
   
   //verifica se o numero de threads foi passado na linha de comando
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]); return 1;
   }
   nthreads = atoi(argv[1]);

   //insere os primeiros elementos na lista
   for(int i=0; i<QTDE_INI; i++)
      Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */

   //aloca espaco de memoria para o vetor de identificadores de threads no sistema
   tid = malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {  
      printf("--ERRO: malloc()\n"); return 2;
   }

   //tomada de tempo inicial
   GET_TIME(ini);
   //inicializa a variavel mutex e as variaveis de condição
   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&condEscrita, NULL);
   pthread_cond_init(&condLeitura, NULL);
   
   //cria as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)) {
         printf("--ERRO: pthread_create()\n"); return 3;
      }
   }
   
   //aguarda as threads terminarem
   for(int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), NULL)) {
         printf("--ERRO: pthread_join()\n"); return 4;
      }
   }

   //tomada de tempo final
   GET_TIME(fim);
   delta = fim-ini;
   printf("Tempo: %lf\n", delta);

   //libera o mutex e as condições
   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&condEscrita);
   pthread_cond_destroy(&condLeitura);
   //libera o espaco de memoria do vetor de threads
   free(tid);
   //libera o espaco de memoria da lista
   Free_list(&head_p);

   return 0;
}
