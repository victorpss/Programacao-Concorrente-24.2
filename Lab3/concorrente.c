#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

int produtosInternosRealizados = 0;

typedef struct {
   int idThread;
   float *m1, *m2; // matrizes de entrada
   long long int qtdeProdutosATratar, qtdeProdutosJaTratados;
   int linhas1, colunas1, linhas2, colunas2;
} t_Args;

typedef struct {
   float *resultado; // matriz resultante
} t_Ret;

void* multiplicacaoConcorrente(void* arg) {
   float soma;
   t_Args args = *(t_Args*)arg;
   float *mat1 = args.m1; // primeira matriz
   float *mat2 = args.m2; // segunda matriz

   t_Ret *ret = malloc(sizeof(t_Ret));
   if (ret == NULL) {
      printf("--ERRO: malloc() thread\n");
      pthread_exit(NULL);
   }
   ret->resultado = calloc(args.qtdeProdutosATratar, sizeof(float)); // alocando memória para o resultado

   //para calcular a partir de que linha e coluna devo começar a operaçao, ja que podemos ter varias threads
   long long int inicio = args.qtdeProdutosJaTratados;
   long long int fim = inicio + args.qtdeProdutosATratar;

   for (long long int ind = inicio; ind < fim; ind++) {
      int i = ind / args.colunas2; // linha correspondente da primeira matriz para produto vetorial
      int j = ind % args.colunas2; // coluna correspondente da segunda matriz para produto vetorial
      soma = 0;

      for (int k = 0; k < args.colunas1; k++) {
         soma += *(mat1 + (i * args.colunas1 + k)) * *(mat2 + (k * args.colunas2 + j)); // produto vetorial
      }

      ret->resultado[ind - inicio] = soma; //guardando o elemento calculado
   }

   pthread_exit((void*)ret);
   return (void*) ""; //só para não ter warning
}

int main(int argc, char *argv[]) {
   float *matriz1, *matriz2; // matrizes que serão carregadas do arquivo de entrada
   float *resultadoConcorrente; // matriz resultado
   int linhas1, colunas1, linhas2, colunas2; // dimensões das matrizes
   int nthreads;
   long long int tam; // qtde de elementos na matriz
   FILE *arquivoEntrada1, *arquivoEntrada2, *arquivoSaida; // descritores dos arquivos de entrada e de saída
   size_t ret; // retorno da função de leitura no arquivo de entrada
   pthread_t *tid_sistema; // vetor de identificadores das threads no sistema
   t_Args *args;
   t_Ret *retorno;
   double inicio, fim, delta;

   GET_TIME(inicio);
   
   // Recebe os argumentos de entrada
   if (argc < 5) {
      fprintf(stderr, "Digite: %s <arquivo entrada 1> <arquivo entrada 2> <arquivo saída> <qtde de threads>\n", argv[0]);
      return 1;
   }
   
   nthreads = atoi(argv[4]);
   
   // Abre o arquivo para leitura binária
   arquivoEntrada1 = fopen(argv[1], "rb");
   if (!arquivoEntrada1) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 2;
   }

   // Lê as dimensões da matriz
   ret = fread(&linhas1, sizeof(int), 1, arquivoEntrada1);
   if (!ret) {
      fprintf(stderr, "Erro de leitura das dimensões da matriz arquivo\n");
      return 3;
   }
   ret = fread(&colunas1, sizeof(int), 1, arquivoEntrada1);
   if (!ret) {
      fprintf(stderr, "Erro de leitura das dimensões da matriz arquivo\n");
      return 3;
   }

   // Aloca memória para a matriz 1
   matriz1 = (float*)malloc(sizeof(float) * linhas1 * colunas1);
   if (!matriz1) {
      fprintf(stderr, "Erro de alocação da memória da matriz 1\n");
      return 3;
   }

   // Carrega a matriz de elementos do tipo float do arquivo
   ret = fread(matriz1, sizeof(float), linhas1 * colunas1, arquivoEntrada1);
   if (ret < linhas1 * colunas1) {
      fprintf(stderr, "Erro de leitura dos elementos da matriz 1\n");
      return 4;
   }

   // Abre o arquivo para leitura binária
   arquivoEntrada2 = fopen(argv[2], "rb");
   if (!arquivoEntrada2) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 2;
   }

   // Lê as dimensões da matriz
   ret = fread(&linhas2, sizeof(int), 1, arquivoEntrada2);
   if (!ret) {
      fprintf(stderr, "Erro de leitura das dimensões da matriz arquivo\n");
      return 3;
   }
   ret = fread(&colunas2, sizeof(int), 1, arquivoEntrada2);
   if (!ret) {
      fprintf(stderr, "Erro de leitura das dimensões da matriz arquivo\n");
      return 3;
   }

   // Aloca memória para a matriz 2
   matriz2 = (float*)malloc(sizeof(float) * linhas2 * colunas2);
   if (!matriz2) {
      fprintf(stderr, "Erro de alocação da memória da matriz 2\n");
      return 3;
   }

   // Carrega a matriz de elementos do tipo float do arquivo
   ret = fread(matriz2, sizeof(float), linhas2 * colunas2, arquivoEntrada2);
   if (ret < linhas2 * colunas2) {
      fprintf(stderr, "Erro de leitura dos elementos da matriz 2\n");
      return 4;
   }

   if(colunas1 != linhas2){
      fprintf(stderr, "A dimensão das matrizes de entrada impedem a realização da multiplicação.\nA quantidade de colunas da primeira matriz deve ser igual à quantidade de linhas da segunda matriz. \n");
      return 6;
   }

   tam = linhas1 * colunas2; // calcula a qtde de elementos da matriz

   GET_TIME(fim);

   delta = fim - inicio;

   printf("----MULTIPLICAÇÃO CONCORRENTE----\n\n");
   printf("Tempo para inicialização: %.30lf\n", delta);

   GET_TIME(inicio);

   tid_sistema = (pthread_t*)malloc(sizeof(pthread_t) * nthreads);
   if (tid_sistema == NULL) {
      fprintf(stderr, "--ERRO: malloc()\n");
      return 5;
   }

   resultadoConcorrente = (float*)malloc(sizeof(float) * tam);
   if (!resultadoConcorrente) {
      fprintf(stderr, "Erro de alocação da memória para a matriz resultado\n");
      return 5;
   }

   long long int min = floor(tam / nthreads); // mínimo de produtos internos a serem tratados por cada thread
   long long int max = min + 1; // pois quero equilibrar a quantidade de multiplicações para as threads

   int sobrecarregadas = tam % nthreads; // qtde daqueles que vão calcular [min+1] produtos internos
   int carregadas = nthreads - sobrecarregadas;

   for (long int i = 0; i < nthreads; i++) {
      args = malloc(sizeof(t_Args));

      if (args == NULL) {
         printf("--ERRO: malloc()\n");
         return 2;
      }
      args->idThread = i;
      args->m1 = matriz1;
      args->m2 = matriz2;
      args->qtdeProdutosATratar = (i < carregadas) ? min : max;
      args->qtdeProdutosJaTratados = (i < carregadas) ? i * min : carregadas * min + (i - carregadas) * max; // calculando o total de produtos vetoriais calculados até aqui
      args->linhas1 = linhas1;
      args->colunas1 = colunas1;
      args->linhas2 = linhas2;
      args->colunas2 = colunas2;

      if (pthread_create(&tid_sistema[i], NULL, multiplicacaoConcorrente, (void*)args)) {
         printf("--ERRO: pthread_create()\n");
         exit(-1);
      }
   }

   for (int i = 0; i < nthreads; i++) {
      if (pthread_join(tid_sistema[i], (void**)&retorno)) {
         printf("--ERRO: pthread_join() da thread %d\n", i);
      }

      // calculando o índice inicial e a quantidade de produtos vetoriais para a thread
      long long int inicio;
      long long int qtdProdutos;

      inicio = (i < carregadas) ? i * min : carregadas * min + (i - carregadas) * max;
      qtdProdutos = (i < carregadas) ? min : max;

      // copiando os resultados de cada thread para a matriz resultante na main
      for (long long int j = 0; j < qtdProdutos; j++) {
         resultadoConcorrente[inicio + j] = retorno->resultado[j];
      }

      free(retorno);
   }

   GET_TIME(fim);

   delta = fim - inicio;

   printf("Tempo para processamento: %.30lf\n", delta);

   GET_TIME(inicio);

   arquivoSaida = fopen(argv[3], "wb");
   if(!arquivoSaida) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 2;
   }

   // escreve numero de linhas e de colunas da matriz resultante
   ret = fwrite(&linhas1, sizeof(int), 1, arquivoSaida);
   ret = fwrite(&colunas2, sizeof(int), 1, arquivoSaida);
   // escreve os elementos da matriz resultante
   ret = fwrite(resultadoConcorrente, sizeof(float), tam, arquivoSaida);
   if(ret < tam) {
      fprintf(stderr, "Erro de escrita no arquivo\n");
      return 4;
   }

   free(matriz1);
   free(matriz2);
   free(resultadoConcorrente);
   free(tid_sistema);

   GET_TIME(fim);

   delta = fim - inicio;

   printf("Tempo para finalização: %.30lf\n", delta);

   return 0;
}
