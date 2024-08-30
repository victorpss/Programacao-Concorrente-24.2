/* Programa que cria arquivo com uma matriz de valores do tipo float, gerados aleatoriamente 
 * Entrada: dimensoes da matriz (n e m) e nome do arquivo de saida
 * Saida: arquivo binario com as dimensoes (valores inteiros) da matriz (n e m), 
 * seguido dos valores (float) de todas as celulas da matriz gerados aleatoriamente
 * */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char*argv[]) {
   float *matriz1; //matriz que será gerada
   int n, m; //dimensoes da matriz
   long long int tam; //qtde de elementos na matriz
   FILE * descritorArquivo; //descritor do arquivo de saida
   size_t ret; //retorno da funcao de escrita no arquivo de saida
   
   //recebe os argumentos de entrada
   if(argc < 4) {
      fprintf(stderr, "Digite: %s <linhas> <colunas> <arquivo saida>\n", argv[0]);
      return 1;
   }
   n = atoi(argv[1]); 
   m = atoi(argv[2]);
   tam = n * m;

   //aloca memoria para a primeira matriz
   matriz1 = (float*) malloc(sizeof(float) * tam);
   if(!matriz1) {
      fprintf(stderr, "Erro de alocao da memoria da matriz 1\n");
      return 2;
   }

   //preenche a primeira matriz com valores float aleatorios
   //randomiza a sequencia de numeros aleatorios
   srand(time(NULL));
   for(long int i=0; i<tam; i++) {
       *(matriz1+i) = (rand() % 1000) * 0.3;
   }

   //escreve a matriz no arquivo
   //abre o arquivo para escrita binaria
   descritorArquivo = fopen(argv[3], "wb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 3;
   }
   //escreve numero de n e de m
   ret = fwrite(&n, sizeof(int), 1, descritorArquivo);
   ret = fwrite(&m, sizeof(int), 1, descritorArquivo);
   //escreve os elementos da primeira matriz
   ret = fwrite(matriz1, sizeof(float), tam, descritorArquivo);
   if(ret < tam) {
      fprintf(stderr, "Erro de escrita no arquivo\n");
      return 4;
   }

   //finaliza o uso das variaveis
   fclose(descritorArquivo);
   free(matriz1);
   return 0;
}