/*
Nome: Victor Pereira da Silva dos Santos
DRE: 122073559
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 300 //valor maximo de um elemento do vetor
//descomentar o define abaixo caso deseje imprimir uma versao do vetor gerado no formato texto
#define TEXTO 

float* geraVetor(int dim){

    float elemento; // elemento que vai entrar no vetor
    float* vetor;

    vetor = (float*) malloc(sizeof(float) * dim);
    if(!vetor) {
        fprintf(stderr, "Erro de alocao da memoria do vetor\n");
        return (float*) 2;
    }   

    for(long int i=0; i<dim; i++) {
            elemento = (rand() % MAX)/6.0;
            vetor[i] = elemento;
    } 

    return vetor;
}

double produtoInternoSequencial(float* vetor1, float* vetor2, int dim){
    double resultado = 0; // alternar entre float e double para ver o erro

    for(int i=0; i<dim; i++){
        resultado += vetor1[i] * vetor2[i];
    }

    return resultado;
}

int main(int argc, char*argv[]) {
    float *vetor1, *vetor2; // vetores que farão parte do produto interno
    long int dim; // dimensao dos vetores
    double resultado=0; //resultado do produto interno, alternar entre float e double para ver o erro
    FILE *arquivo; //descritor do arquivo de saida
    size_t ret; //retorno da funcao de escrita no arquivo de saida

    //recebe os argumentos de entrada
    if(argc < 2) {
        fprintf(stderr, "Digite: %s <dimensao> \n", argv[0]);
        return 1;
    }

    dim = atoi(argv[1]);

    // para gerar vetores diferentes. Se eu jogasse na função de gerar vetor, os vetores seriam iguais porque srand iria contar o mesmo periodo de tempo para gerar os elementos dos diferentes vetores, consequentemente, os valores seriam iguais.
    srand(time(NULL)); 

    vetor1 = geraVetor(dim);
    vetor2 = geraVetor(dim);

    //imprimir na saida padrao o vetor gerado
    #ifdef TEXTO
    fprintf(stdout, "Dimensão dos vetores: %ld\n\n", dim);

    // se quiser ver os elementos dos vetores, descomente o bloco abaixo;
    /*
    fprintf(stdout, "Vetor 1:\n");
    for(long int i=0; i<dim; i++) {
        fprintf(stdout, "%f ",vetor1[i]);
    }
    fprintf(stdout, "\n\n");
    fprintf(stdout, "Vetor 2:\n");
    for(long int i=0; i<dim; i++) {
        fprintf(stdout, "%f ",vetor2[i]);
    }
    fprintf(stdout, "\n\n");
    */
    #endif

    arquivo = fopen("vetores", "wb");
    if(!arquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }
    //escreve a dimensão
    ret = fwrite(&dim, sizeof(long int), 1, arquivo);
    //escreve os elementos dos vetores
    ret = fwrite(vetor1, sizeof(float), dim, arquivo);
    ret = fwrite(vetor2, sizeof(float), dim, arquivo);
    if(ret < dim) {
        fprintf(stderr, "Erro de escrita no arquivo\n");
        return 4;
    }

    //calculo do produto interno de forma sequencial
    resultado = produtoInternoSequencial(vetor1, vetor2, dim);
    fprintf(stdout, "Produto interno sequencial: %lf ", resultado);
    ret = fwrite(&resultado, sizeof(double), 1, arquivo); // alternar entre float e double para ver o erro


    //fecha arquivo e libera ponteiros
    fclose(arquivo);
    free(vetor1);
    free(vetor2);
    return 0;
} 
