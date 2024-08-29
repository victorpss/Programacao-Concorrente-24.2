/* Programa que le um arquivo binario com dois valores inteiros (indicando as dimensoes de uma matriz) 
 * e uma sequencia com os valores da matriz (em float)
 * Entrada: nome do arquivo de entrada
 * Saida: valores da matriz escritos no formato texto (com 6 casas decimais) na saida padrao 
 * */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct{
   float *m1, *m2;
   int qtdeVetoresATratar, qtdeVetoresJaTratados;
} t_Args;

typedef struct{
   float resultado;
} t_Ret;

void* multiplicacaoConcorrente(void* arg){
   t_Args args = *(t_Args*) arg;

   t_Ret *ret = malloc(sizeof(t_Ret));
   if (ret==NULL) {
        printf("--ERRO: malloc() thread\n");
        pthread_exit(NULL);
   }

   pthread_exit(NULL);
}

float* multiplicacaoSequencial(float *m1, float *m2, int linhas, int colunas){
    int tam = linhas * colunas;
    float *res = malloc(sizeof(float) * tam); // m1: n x m; m2: m x n; dimensao resultante: n x n
    float soma;
    int aux = 0; //para ajudar a escrever o elemento no vetor res

    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < linhas; j++){
            soma = 0;

            for(int k = 0; k < colunas; k++){
                soma += m1[i*colunas + k] * m2[k*linhas + j];
                //printf("\nm1 = %f, m2 = %f\n", m1[i*colunas + k], m2[k*linhas + j]);
            }

            *(res+aux) = soma;
            aux++;
        }
    }

    return res;
}

int main(int argc, char*argv[]) {
   float *matriz1, *matriz2, *resultado; //matrizes que serão carregadas do arquivo de entrada
   //matriz 1: n x m
   //matriz 2: m x n
   //resultado: n x n
   int n, m; //dimensoes da matriz
   int nthreads;
   long long int tam; //qtde de elementos na matriz
   FILE *arquivoEntrada, *arquivoSaida; //descritores do arquivo de entrada e de saida
   size_t ret; //retorno da funcao de leitura no arquivo de entrada
   pthread_t *tid_sistema; //vetor de identificadores das threads no sistema
   t_Args *args;
   t_Ret *retorno;
   
   //recebe os argumentos de entrada
   if(argc < 4) {
      fprintf(stderr, "Digite: %s <arquivo entrada> <arquivo saida> <qtde de threads>\n", argv[0]);
      return 1;
   }
   
   nthreads = atoi(argv[3]);
   
   //abre o arquivo para leitura binaria
   arquivoEntrada = fopen(argv[1], "rb");
   if(!arquivoEntrada) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 2;
   }

   //le as dimensoes da matriz
   ret = fread(&n, sizeof(int), 1, arquivoEntrada);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
      return 3;
   }
   ret = fread(&m, sizeof(int), 1, arquivoEntrada);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
      return 3;
   }
   tam = n * m; //calcula a qtde de elementos da matriz

   //aloca memoria para a matriz 1
   matriz1 = (float*) malloc(sizeof(float) * tam);
   if(!matriz1) {
      fprintf(stderr, "Erro de alocao da memoria da matriz 1\n");
      return 3;
   }

   //carrega a matriz de elementos do tipo float do arquivo
   ret = fread(matriz1, sizeof(float), tam, arquivoEntrada);
   if(ret < tam) {
      fprintf(stderr, "Erro de leitura dos elementos da matriz 1\n");
      return 4;
   }

   //aloca memoria para a matriz 2
   matriz2 = (float*) malloc(sizeof(float) * tam);
   if(!matriz2) {
      fprintf(stderr, "Erro de alocao da memoria da matriz 2\n");
      return 3;
   }

   //carrega a matriz de elementos do tipo float do arquivo
   ret = fread(matriz2, sizeof(float), tam, arquivoEntrada);
   if(ret < tam) {
      fprintf(stderr, "Erro de leitura dos elementos da matriz 2\n");
      return 4;
   }


   resultado = multiplicacaoSequencial(matriz1, matriz2, n, m);

  /*imprime a matriz na saida padrao
   for(int i=0; i<n; i++) { 
      for(int j=0; j<n; j++)
        fprintf(stdout, "%.6f ", resultado[i*n+j]);
      fprintf(stdout, "\n");
   }*/

   tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);

   if(tid_sistema==NULL){ 
      printf("--ERRO: malloc()\n"); 
      exit(-1); 
   }












   //abre o arquivo para escrita binaria
   arquivoSaida = fopen(argv[2], "wb");
   if(!arquivoSaida) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 2;
   }
   //escreve numero de n e de m
   //ret = fwrite(&n, sizeof(int), 1, arquivoSaida);
   //ret = fwrite(&m, sizeof(int), 1, arquivoSaida);
   //escreve os elementos da matriz resultante
   ret = fwrite(resultado, sizeof(float), tam, arquivoSaida);
   if(ret < tam) {
      fprintf(stderr, "Erro de escrita no arquivo\n");
      return 4;
   }

   //finaliza o uso das variaveis
   fclose(arquivoEntrada);
   fclose(arquivoSaida);
   free(matriz1);
   free(matriz2);
   return 0;
}

