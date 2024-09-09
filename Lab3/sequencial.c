#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

float* multiplicacaoSequencial(float *m1, float *m2, int linhas1, int colunas1, int linhas2, int colunas2){
    int tam = linhas1 * colunas2;
    float *res = malloc(sizeof(float) * tam); // m1: l1 x c1; m2: l2 x c2; dimensao resultante: l1 x c2
    float soma;
    int aux = 0; //para ajudar a escrever o elemento no vetor res

    for(int i = 0; i < linhas1; i++){
        for(int j = 0; j < colunas2; j++){
            soma = 0;

            for(int k = 0; k < colunas1; k++){
                soma += m1[i*colunas1 + k] * m2[k*colunas2 + j];
            }

            *(res+aux) = soma;
            aux++;
        }
    }

    return res;
}

int main(int argc, char*argv[]) {
   float *matriz1, *matriz2, *resultado; //matrizes que serão carregadas do arquivo de entrada
   //matriz 1: n1 x m1
   //matriz 2: n2 x m2
   //resultado: n1 x m2
   int linhas1, colunas1, linhas2, colunas2; //dimensoes das matrizes
   long long int tam; //qtde de elementos na matriz
   FILE *arquivoEntrada1, *arquivoEntrada2, *arquivoSaida; //descritores do arquivo de entrada e de saida
   size_t ret; //retorno da funcao de leitura no arquivo de entrada
   double inicio, fim, delta;

   GET_TIME(inicio);
   
   //recebe os argumentos de entrada
   if(argc < 4) {
      fprintf(stderr, "Digite: %s <arquivo entrada 1> <arquivo entrada 2> <arquivo saida>\n", argv[0]);
      return 1;
   }
   
   //abre o arquivo para leitura binaria da primeira matriz
   arquivoEntrada1 = fopen(argv[1], "rb");
   if(!arquivoEntrada1) {
      fprintf(stderr, "Erro de abertura do arquivo da primeira matriz de entrada\n");
      return 2;
   }

   //le as dimensoes da matriz
   ret = fread(&linhas1, sizeof(int), 1, arquivoEntrada1);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz de entrada 1 \n");
      return 3;
   }
   ret = fread(&colunas1, sizeof(int), 1, arquivoEntrada1);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz de entrada 1 \n");
      return 3;
   }

   //abre o arquivo para leitura binaria da segunda matriz
   arquivoEntrada2 = fopen(argv[2], "rb");
   if(!arquivoEntrada2) {
      fprintf(stderr, "Erro de abertura do arquivo da segunda matriz de entrada\n");
      return 2;
   }

   //le as dimensoes da matriz
   ret = fread(&linhas2, sizeof(int), 1, arquivoEntrada2);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz de entrada 2 \n");
      return 3;
   }
   ret = fread(&colunas2, sizeof(int), 1, arquivoEntrada2);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz de entrada 2 \n");
      return 3;
   }

   if(colunas1 != linhas2){
      fprintf(stderr, "A dimensão das matrizes de entrada impedem a realização da multiplicação.\nA quantidade de colunas da primeira matriz deve ser igual à quantidade de linhas da segunda matriz. \n");
      return 5;
   }

   tam = linhas1 * colunas2; //calcula a qtde de elementos da matriz resultante

   //aloca memoria para a matriz 1
   matriz1 = (float*) malloc(sizeof(float) * linhas1*colunas1);
   if(!matriz1) {
      fprintf(stderr, "Erro de alocao da memoria da matriz 1\n");
      return 3;
   }

   //carrega a matriz de elementos do tipo float do arquivo
   ret = fread(matriz1, sizeof(float), linhas1*colunas1, arquivoEntrada1);
   if(ret < linhas1*colunas1) {
      fprintf(stderr, "Erro de leitura dos elementos da matriz 1\n");
      return 4;
   }

   //aloca memoria para a matriz 2
   matriz2 = (float*) malloc(sizeof(float) * linhas2*colunas2);
   if(!matriz2) {
      fprintf(stderr, "Erro de alocao da memoria da matriz 2\n");
      return 3;
   }

   //carrega a matriz de elementos do tipo float do arquivo
   ret = fread(matriz2, sizeof(float), linhas2*colunas2, arquivoEntrada2);
   if(ret < linhas2*colunas2) {
      fprintf(stderr, "Erro de leitura dos elementos da matriz 2\n");
      return 4;
   }

   GET_TIME(fim);

   delta = fim - inicio;

   printf("----MULTIPLICAÇÃO SEQUENCIAL----\n\n");
   printf("Tempo para inicialização: %.30lf\n", delta);

   GET_TIME(inicio);
   resultado = multiplicacaoSequencial(matriz1, matriz2, linhas1, colunas1, linhas2, colunas2);
   GET_TIME(fim);

   delta = fim - inicio;

   printf("Tempo para processamento: %.30lf\n", delta);

   GET_TIME(inicio);
   //abre o arquivo para escrita binaria
   arquivoSaida = fopen(argv[3], "wb");
   if(!arquivoSaida) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 2;
   }

   //escreve numero de linhas e de colunas da matriz resultante
   ret = fwrite(&linhas1, sizeof(int), 1, arquivoSaida);
   ret = fwrite(&colunas2, sizeof(int), 1, arquivoSaida);
   //escreve os elementos da matriz resultante
   ret = fwrite(resultado, sizeof(float), tam, arquivoSaida);
   if(ret < tam) {
      fprintf(stderr, "Erro de escrita no arquivo\n");
      return 4;
   }

   //finaliza o uso das variaveis
   fclose(arquivoEntrada1);
   fclose(arquivoEntrada2);
   fclose(arquivoSaida);
   free(matriz1);
   free(matriz2);

   GET_TIME(fim);

   delta = fim - inicio;
   printf("Tempo para finalização: %.30lf\n", delta);

   return 0;
}
