#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

long int soma = 0; //variavel compartilhada entre as threads
int contador = 0; //variavel para contar a qtde de multiplos de 10 impressos
pthread_mutex_t mutex; //variavel de lock para exclusao mutua
pthread_cond_t cond_soma;
pthread_cond_t cond_extra;

//funcao executada pelas threads
void *ExecutaTarefa (void *arg) {
  long int id = (long int) arg;
  printf("Thread : %ld esta executando...\n", id);

  for (int i=0; i<100000; i++) {
     //--entrada na SC
     pthread_mutex_lock(&mutex);
     //--SC (seção critica)
     printf("soltei a thread extra");
     pthread_cond_signal(&cond_extra);
     if(!(soma % 10) && contador < 20){ // se cheguei a um multiplo de 10 e é um dos 20 primeiros
      printf("cheguei aqui e prendi a thread %ld com contador = %d e soma = %ld\n", id, contador, soma);
      pthread_cond_wait(&cond_soma, &mutex); // bloqueio para printar o valor de soma
     } 
     printf("thread %ld acabou de somar 1 à soma\n", id);
     soma++; //incrementa a variavel compartilhada
     //--saida da SC
     pthread_mutex_unlock(&mutex);
  }
  printf("Thread : %ld terminou!\n", id);
  pthread_exit(NULL);
}

//funcao executada pela thread de log
void *extra (void *args) {
  printf("Extra : esta executando...\n");
  for (int i=0; i<10000; i++) {
    pthread_mutex_lock(&mutex);
    
    if (!(soma % 10)){ //imprime se 'soma' for multiplo de 10
      printf("soma = %ld, i = %d \n", soma, i); //não impede de printar o mesmo número repetidamente, porque a thread da soma pode entrar no processador diversas vezes antes da thread de incrementar a soma.
      contador++;


      printf("soltei a thread soma\n");
      pthread_cond_broadcast(&cond_soma); // libero as threads que estão somando

    }
      if(contador < 20){
        printf("prendi a thread extra\n");
        pthread_cond_wait(&cond_extra, NULL);
      }
    pthread_mutex_unlock(&mutex);
  }
  printf("Extra : terminou!\n");
  pthread_exit(NULL);
}

//fluxo principal
int main(int argc, char *argv[]) {
   pthread_t *tid; //identificadores das threads no sistema
   int nthreads; //qtde de threads (passada linha de comando)

   //--inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao
   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&cond_soma, NULL);
   pthread_cond_init(&cond_extra, NULL);

   //--le e avalia os parametros de entrada
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]);
      return 1;
   }
   nthreads = atoi(argv[1]);

   //--aloca as estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads+1));
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}

   //--cria as threads
   for(long int t=0; t<nthreads; t++) {
     if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)t)) {
       printf("--ERRO: pthread_create()\n"); exit(-1);
     }
   }

   //--cria thread de log
   if (pthread_create(&tid[nthreads], NULL, extra, NULL)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
   }

   //--espera todas as threads terminarem
   for (int t=0; t<nthreads+1; t++) {
     if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
     } 
   } 

   //--finaliza o mutex e o cond
   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&cond_soma);
   pthread_cond_destroy(&cond_extra);
   
   printf("Valor de 'soma' = %ld\n", soma);

   return 0;
}