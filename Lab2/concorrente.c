/*
Nome: Victor Pereira da Silva dos Santos
DRE: 122073559
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <pthread.h> 

//descomentar o define abaixo caso deseje imprimir uma versao do vetor gerado no formato texto
#define TEXTO 

// argumentos de entrada para funçao de calcular produto interno
typedef struct {
   int idThread;
   float *vetor1, *vetor2;
   int qtdeElementosATratar, qtdeElementosJaTratados;
} t_Args;

typedef struct {
   double res;
} t_Ret;


void* produtoInternoConcorrente(void* arg){
    t_Ret *ret; //estrutura de retorno

    double resultadoParcial = 0;
    t_Args args = *(t_Args*) arg;

    for(int i = 0; i < args.qtdeElementosATratar; i++){
        resultadoParcial += args.vetor1[args.qtdeElementosJaTratados + i] * args.vetor2[args.qtdeElementosJaTratados + i];
        //na linha abaixo dá pra ter uma ideia das threads multiplicando os elementos e acumulando os resultados das operações 
        //printf("Thread %d, elemento %d -> %lf\n", args.idThread, args.qtdeElementosJaTratados + i, resultadoParcial);
    }

    ret = malloc(sizeof(t_Ret));
    if (ret==NULL) {
        printf("--ERRO: malloc() thread\n");
        pthread_exit(NULL);
    }
    
    ret->res = resultadoParcial;

    free(arg);

    pthread_exit((void *) ret);

    return (void*) ""; //só para não ter warning
}

int main(int argc, char*argv[]) {
    long int nthreads; // quantidade de threads
    long int dim; // dimensão dos vetores

    double resultadoConcorrente; // resultado do produto interno concorrente
    double resultadoSequencial; // resultado do produto interno sequencial

    float *vetor1, *vetor2; // vetores que farão parte do produto interno

    FILE *arquivo; //descritores do arquivo de saida
    size_t ret; //retorno da funcao de escrita no arquivo de saida
    pthread_t *tid_sistema; //vetor de identificadores das threads no sistema
    t_Args *args;
    t_Ret *retorno; // struct com resultado do produto interno realizado por cada thread

    //recebe os argumentos de entrada
    if(argc < 3) {
        fprintf(stderr, "Digite: %s <nthreads> <arquivo> \n", argv[0]);
        return 1;
    }

    nthreads = atoi(argv[1]);
    arquivo = fopen(argv[2], "rb");

    if(arquivo==NULL){ 
        printf("--ERRO: fopen()\n"); 
        exit(-1); 
    }

    //pegando a dimensao dos vetores
    ret = fread(&dim, sizeof(long int), 1, arquivo);

    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }

    if(nthreads > dim){
        nthreads = dim; // limitando a quantidade de threads à dimensão do vetor
    }

    vetor1 = malloc (sizeof(float) * dim);

    if(vetor1==NULL){ 
        printf("--ERRO: malloc()\n"); 
        exit(-1);
    }

    vetor2 = malloc (sizeof(float) * dim);

    if(vetor2==NULL){ 
        printf("--ERRO: malloc()\n"); 
        exit(-1);
    }

    //pegando os vetores
    ret = fread(vetor1, sizeof(float), dim, arquivo);
    ret = fread(vetor2, sizeof(float), dim, arquivo);

    if(ret < dim) {
        fprintf(stderr, "Erro de leitura dos elementos do vetor\n");
        return 4;
    }

    //pegando o resultado do produto interno sequencial
    ret = fread(&resultadoSequencial, sizeof(double), 1, arquivo);

    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);

    if(tid_sistema==NULL){ 
        printf("--ERRO: malloc()\n"); 
        exit(-1); 
    }

    int min = floor(dim/nthreads); // mínimo de elementos (em um vetor) a serem tratados por cada thread
    int max = min + 1; // pois quero equilibrar a quantidade de elementos para as threads

    // inicialmente, distribuimos [min] elementos para cada thread, porém, ainda pode sobrar elementos (resto) pois a divisão de elementos por thread pode não ser inteira,
    // então tenho que ver quantas threads vão ter que receber um elemento a mais para ser tratado, chamarei elas de 'sobrecarregadas', enquanto isso chamarei de 'carregadas' a qtde de threads que vão tratar [min + 1] elementos
    int sobrecarregadas = dim % nthreads; // qtde daqueles que vão tratar [min+1] elementos. Se sobrecarregadas == 0, então quer dizer que consegui distribuir igualmente a quantidade de elementos para cada thread.
    int carregadas = nthreads - sobrecarregadas;

    int preenchidos = 0; // variável auxiliar para manter a quantidade de elementos preenchidos atualizada

    for(long int i=0; i<nthreads; i++) {
        args = malloc(sizeof(t_Args));

        if (args == NULL) {
            printf("--ERRO: malloc()\n"); 
            return 2;
        }
        args->idThread = i; 
        args->vetor1 = vetor1;
        args->vetor2 = vetor2;
        args->qtdeElementosJaTratados = preenchidos;
        args->qtdeElementosATratar = (i+1 <= carregadas) ? min : max;

        if (pthread_create(&tid_sistema[i], NULL, produtoInternoConcorrente, (void*) args)) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }

        preenchidos += args->qtdeElementosATratar; // atualizando a variável auxiliar após realizar a operação
    }
    
    for (int i=0; i<nthreads; i++) {
        if (pthread_join(tid_sistema[i], (void**) &retorno)) {
            printf("--ERRO: pthread_join() da thread %d\n", i);
        }
        resultadoConcorrente += retorno->res;
        free(retorno);
   }

    double variacaoRelativa = abs((resultadoSequencial - resultadoConcorrente)/resultadoSequencial);
    printf("Resultado do produto interno concorrente: %.26lf\nResultado do produto interno sequencial: %.26lf\nVariação relativa: %.26lf\n\n", resultadoConcorrente, resultadoSequencial, variacaoRelativa);

    //log da função main
    printf("--Thread principal terminou\n");

    free(vetor1);
    free(vetor2);
    free(tid_sistema);
    fclose(arquivo);
    return 0;
} 
