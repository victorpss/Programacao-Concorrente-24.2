#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 3
#define TAM 10 // tamanho de N

// Variaveis globais
sem_t sem1, sem2, sem3;      // semaforos para coordenar a ordem de execucao das threads
char buffer1[TAM+1]; // a cada leitura com o fgets, eu leio TAM+1 elementos (o último é sempre '\0')
char buffer2[4*TAM]; // buffer com espaço suficiente para armazenar o buffer1 modificado
int contador = 0; //conta caracteres já inseridos no buffer2
int fim_arquivo = 0; // flag para indicar o fim do arquivo

//lendo N caracteres e carregando no buffer1
void *t1 (void *arg) {
    FILE *entrada = (FILE*) arg;

    while(fgets(buffer1, TAM+1, entrada) != NULL){
        // li TAM caracteres na condicao e botei no buffer1
        printf("buf1: %s\n", buffer1);

        // liberar t2
        sem_post(&sem2);
        // parar para a t2 fazer algo
        sem_wait(&sem1);
    }

    // quando terminar de ler o arquivo todo, insiro um nulo para indicar fim do buffer e libero a t3 de escrever no arquivo
    fclose(entrada);
    fim_arquivo = 1;
    sem_post(&sem2); // liberar t2 para saber que terminou
    sem_post(&sem3); // liberar t3 para escrever no arquivo

    pthread_exit(NULL);
}

//ler o buffer1 e passar pro buffer2 de forma modificada
void *t2 (void *arg) {
    int i = 0;
    int n = 0; // conta quantos '\n' já foram adicionados mas se limita a 10 devido ao enunciado
    int index = 0;
    int prox = 2*n+1; // prox indica o char que irei ler e adicionar um '\n' em seguida

    while(1) {
        // bloquear t2 se buffer1 não está pronto para ser lido
        char atual;
        sem_wait(&sem2);

        if (fim_arquivo) break; // se o arquivo terminou, saímos do loop

        for(i = 0; i < TAM; i++){ //rodando por todos os caracteres de buffer1 (i pode servir como contador de caracteres lidos)
            atual = buffer1[i];

            if(index == prox){
                buffer2[i] = '\n'; //adiciona o \n ao lugar apropriado
                index = 0;
                prox = (n < 10) ? (2*(++n)+1) : i + 10;
                contador++;
            }

            buffer2[contador+i+n] = atual;
            index++;
        }

        printf("buf2: %s\n", buffer2);
        contador += i+n;
        sem_post(&sem1); // devolvo para t1 ler mais um pacote de caracteres
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

    if(argc<1) {
        printf("Digite: %s <arquivo de entrada>\n", argv[0]);
        return 1;
    }

    entrada = fopen("entrada.txt", "r");
    if (!entrada) {
        fprintf(stderr, "Erro de abertura do arquivo de entrada\n");
        return 2;
    }

    //inicia os semaforos
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
