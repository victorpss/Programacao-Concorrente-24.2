#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//comente a linha abaixo para esconder a ordem de execução e terminação das threads, descomente para visualizá-las
#define THREADS

long int soma = 0; //variavel compartilhada entre as threads
int contador = 0; //variavel para contar a qtde de multiplos de 10 impresso
pthread_mutex_t mutex; //variavel de lock para exclusao mutua
pthread_cond_t cond_soma; //condicional para a thread de soma
pthread_cond_t cond_extra; //condicional para a thread extra de print

//função das threads que incrementam soma
void *ExecutaTarefa (void *arg) {
    long int id = (long int) arg;
 
    #ifdef THREADS
    printf("Thread : %ld esta executando...\n", id);
    #endif

    for (int i=0; i<10000; i++) {
        //--entrada na SC
        pthread_mutex_lock(&mutex);
        //--SC (seção critica)
        while(soma == contador * 10 && contador < 20){ // se estou em um multiplo de 10 e é um dos primeiros
            pthread_cond_signal(&cond_extra); // permite o print
            pthread_cond_wait(&cond_soma, &mutex); // bloqueia para printar o valor de soma
        }
        soma++; //incrementa a variavel compartilhada (como ela está sendo incrementada após a checagem, o primeiro múltiplo checado é 0, e, na nossa definição, 0 é múltiplo de 10 e será printado)
        //--saida da SC
        pthread_mutex_unlock(&mutex);
    }

  #ifdef THREADS
  printf("Thread : %ld terminou!\n", id);
  #endif

  pthread_exit(NULL);
}

//função da thread de print da soma
void *extra (void *args) {
  #ifdef THREADS
  printf("Extra : esta executando...\n");
  #endif
  printf("\n");
 
  for(int i=0; i<10000 && contador < 20; i++){
    pthread_mutex_lock(&mutex); // entrada na seção crítica
   
    if (!(soma%10) && soma == contador * 10){ // imprime se 'soma' for multiplo de 10 e é igual ao contador atual multiplicado por 10 (garante print unico do valor da soma)
      printf("soma = %ld \n", soma);
      contador++;
    }
    else{ // se nao é multiplo ou se ele já foi printado, a extra é bloqueada ate chegar algum novo multiplo (e as threads de soma serão desbloqueadas)
       pthread_cond_broadcast(&cond_soma);
       pthread_cond_wait(&cond_extra, &mutex);
    }

    pthread_mutex_unlock(&mutex); // saída da seção crítica
  }
    pthread_cond_broadcast(&cond_soma); // para garantir que nenhuma thread ficará bloqueada

    #ifdef THREADS
    printf("\nExtra : terminou!\n");
    #endif

    pthread_exit(NULL);
}

//fluxo principal
int main(int argc, char *argv[]) {
   pthread_t *tid; //identificadores das threads no sistema
   int nthreads; //qtde de threads (passada linha de comando)

   //--inicilaiza o mutex (lock de exclusao mutua) e as variaveis de condicao
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

   //--finaliza o mutex e as condições
   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&cond_soma);
   pthread_cond_destroy(&cond_extra);
   
   printf("\nValor de 'soma' = %ld\n", soma);

   return 0;
}