#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 3
// descomente/comente a linha abaixo para ativar/desativar o log do buffer1 e perceba que a quantidade de caracteres respeita o tamanho do buffer passado na linha de comando
//#define LOG 


// Variaveis globais
sem_t sem1, sem2, sem3;      // semaforos para coordenar a ordem de execucao das threads
char *buffer1; // buffer para armazenar alguns caracteres do arquivo de entrada
char buffer2[9999]; // buffer com espaço suficiente para armazenar o buffer1 modificado
int contador = 0; // conta caracteres já inseridos no buffer2
int fim_arquivo = 0; // flag para indicar o fim do arquivo
int n = 0; // n do enunciado
int tam; // tamanho do buffer1

//lendo N caracteres e carregando no buffer1
void *t1 (void *arg) {
    FILE *entrada = (FILE*) arg;

    while(fgets(buffer1, tam+1, entrada) != NULL){
        // li tam caracteres na condicao e botei no buffer1
        #ifdef LOG
            printf("buf1: %s\n", buffer1);
        #endif

        // liberar t2
        sem_post(&sem2);
        // parar para a t2 fazer algo
        sem_wait(&sem1);
    }

    // quando terminar de ler o arquivo todo, libero a t3 de escrever no arquivo de saida
    fclose(entrada);
    fim_arquivo = 1;
    sem_post(&sem2); // liberar t2 para saber que terminou
    sem_post(&sem3); // liberar t3 para escrever no arquivo

    pthread_exit(NULL);
}

//ler o buffer1 e passar pro buffer2 de forma modificada
void *t2(void *arg) {
    int index = 0; // índice local para inserir no buffer2
    int prox = 2 * n + 1; // próxima posição para adicionar '\n'

    while (1) {
        sem_wait(&sem2); // esperar buffer1 pronto

        if (fim_arquivo) break;

        for (int i = 0; i < tam; i++) {
            char atual = buffer1[i];

            if (index == prox) {
                buffer2[contador++] = '\n';
                index = 0;

                // prox = quantos índices depois devo adicionar após o índice do último \n
                prox = (n < 10) ? 2 * (++n) + 1 : 10;
            }

            // inserindo o caractere atual no buffer2
            buffer2[contador++] = atual;
            index++;
        }

        sem_post(&sem1); // liberar t1 para ler mais caracteres
    }

    pthread_exit(NULL);
}

//imprimir buffer2 e gerar arquivo de saída
void *t3 (void *arg) {
    sem_wait(&sem3); // aguardar até que o buffer esteja pronto para ser escrito

    FILE *saida = fopen("saida.txt", "w");
    if (!saida) {
        fprintf(stderr, "Erro de abertura do arquivo de saida\n");
        pthread_exit(NULL);
    }

    for (int i = 0; i < contador; i++) {
        printf("%c", buffer2[i]); // imprime no console
        fwrite(&buffer2[i], sizeof(char), 1, saida); // escreve no arquivo
    }

    fclose(saida);

    pthread_exit(NULL);
}

//funcao principal
int main(int argc, char *argv[]) {
    pthread_t tid[NTHREADS];
    FILE *entrada;

    if(argc<3) {
        printf("Digite: %s <tamanho do buffer1> <arquivo de entrada>\n", argv[0]);
        return 1;
    }

    tam = atoi(argv[1]);

    // alocando memória para o buffer1
    buffer1 = (char*) malloc((tam + 1) * sizeof(char));
    if (!buffer1) {
        fprintf(stderr, "Erro ao alocar memória para buffer1\n");
        return 3;
    }

    entrada = fopen(argv[2], "r");
    if (!entrada) {
        fprintf(stderr, "Erro de abertura do arquivo de entrada\n");
        return 2;
    }

    //inicia os semaforos, todos com sinal = 0
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
    sem_init(&sem3, 0, 0);

        //cria as tres threads
    if (pthread_create(&tid[2], NULL, t3, NULL)) { 
        printf("--ERRO: pthread_create()\n"); exit(-1); }
    if (pthread_create(&tid[1], NULL, t2, NULL)) { 
        printf("--ERRO: pthread_create()\n"); exit(-1); }
    if (pthread_create(&tid[0], NULL, t1, (void*) entrada)) { 
        printf("--ERRO: pthread_create()\n"); exit(-1); }

    //--espera todas as threads terminarem
    for (int t=0; t<NTHREADS; t++) {
        if (pthread_join(tid[t], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    } 

    // destruir os semáforos
    sem_destroy(&sem1);
    sem_destroy(&sem2);
    sem_destroy(&sem3);

    return 0;
}
