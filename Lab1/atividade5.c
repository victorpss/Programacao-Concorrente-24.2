/*
Nome: Victor Pereira da Silva dos Santos
DRE: 122073559
*/

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread, qtdeElementosJaPreenchidos, qtdeElementosAPreencher, *vetor;
} t_Args;

int* iniciarVetor (int* vetor, int nelementos){

    printf("-----INICIALIZANDO VETOR-----\n\n");
    //iniciando todos os elementos do vetor com 0
    for(int i = 0; i<nelementos; i++){  
        vetor[i] = 0;
        printf("[%d] - %d\n", i, vetor[i]);
    }

    return vetor;
}

int checarVetor (int* vetor, int tamanho){
  
    // espero que todos os elementos do vetor tenham valor 1
    for(int i = 0; i < tamanho; i++){
        if(vetor[i] != 1){
            return 0; // fracasso
        }
    }
    return 1; // sucesso
}

//funcao executada pelas threads
void *AdicionarUm (void *arg) {
  t_Args *args = (t_Args *) arg;

  // valor antigo: valor novo:
  printf("\nA thread %d somou 1 a %d elemento(s).\n\n", args->idThread, args->qtdeElementosAPreencher);

  for(int i = 0; i < args->qtdeElementosAPreencher; i++){
    args->vetor[args->qtdeElementosJaPreenchidos + i] += 1;
  }

  free(arg); //libera a alocacao feita na main

  pthread_exit(NULL);
}

//funcao principal do programa
int main(int argc, char* argv[]) {
  t_Args *args; //receberá os argumentos para a thread

  int mthreads; //qtde de threads que serao criadas (recebida na linha de comando)
  int nelementos; //qtde de elementos que o vetor irá conter (recebida na linha de comando)

  //verifica se os argumentos 'qtde de threads' e 'qtde de elementos' foram passados e armazena seu valor
  if(argc<3) {
      printf("--ERRO: informe a qtde de threads e a qtde de elementos do vetor <%s> <mthreads> <nelementos>\n", argv[0]);
      return 1;
  }

  mthreads = atoi(argv[1]);
  nelementos = atoi(argv[2]);

  int* vetor = (int *) malloc(nelementos * sizeof(int));

  if(vetor == NULL){
      printf("--ERRO: malloc() do vetor\n"); 
      return 1;
  }

  vetor = iniciarVetor(vetor, nelementos);

  printf("\nTotal de elementos: %d\n", nelementos);
  printf("Total de threads: %d\n\n", mthreads);

  //aqui ja sei mais ou menos a quantos elementos cada thread vai adicionar 1
  int min = floor(nelementos/mthreads); // mínimo de elementos a receberem 1 por cada thread
  int max = min + 1; // pois quero equilibrar a quantidade de elementos para as threads

  // inicialmente, distribuimos [min] elementos para cada thread, porém, ainda pode sobrar elementos (resto) pois a divisão de elementos por thread pode não ser inteira,
  // então tenho que ver quantas threads vão ter que receber um elemento a mais para realizar a soma, chamarei elas de 'sobrecarregadas', enquanto isso chamarei de 'carregadas' a qtde de threads que vão adicionar 1 a [min] elementos
  int sobrecarregadas = nelementos % mthreads; // qtde daqueles que vão adicionar 1 a [min+1] elementos
  int carregadas = mthreads - sobrecarregadas;

  int preenchidos = 0; // variável auxiliar para manter a quantidade de elementos preenchidos atualizada

  //identificadores das threads no sistema
  pthread_t tid_sistema[mthreads];

  //cria as threads
  for(int i=0; i<mthreads; i++) {
    printf("--Aloca e preenche argumentos para thread %d\n", i);
    args = malloc(sizeof(t_Args));
    if (args == NULL) {
      printf("--ERRO: malloc()\n"); 
      return 1;
    }

    args->idThread = i; 
    args->qtdeElementosJaPreenchidos = preenchidos;
    args->qtdeElementosAPreencher = (i+1 <= carregadas) ? min : max;
    args->vetor = vetor;

    printf("--Cria a thread %d\n", i);

    if (pthread_create(&tid_sistema[i], NULL, AdicionarUm, (void*) args)) {
      printf("--ERRO: pthread_create()\n"); 
      return 2;
    }

    preenchidos += args->qtdeElementosAPreencher; // atualizando a variável auxiliar após realizar a soma
  }

  //espera todas as threads terminarem
  for (int i=0; i<mthreads; i++) {
    if (pthread_join(tid_sistema[i], NULL)) {
         printf("--ERRO: pthread_join() da thread %d\n", i); 
         return 3;
    } 
  }

  //log da função principal
  printf("--Thread principal terminou\n\n");

  if(checarVetor(vetor, nelementos)){
    printf("Vetor checado com sucesso. Valores corretos.");
    return 0;
  }

  printf("Vetor retornado com valores incorretos.");
  return 1;
}