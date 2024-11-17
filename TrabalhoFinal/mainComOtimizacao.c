#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#define u_inicial 25.0
#define u_baixo 100.0
#define u_esq 50.0
#define u_cima 0.0
#define u_dir 0.0

#define CARBONO_PIROLITICO 1220
#define DIAMANTE 1100
#define CARBONO 216.5 // 25° C
#define HELIO 190 // 300 K, 1 atm
#define PRATA 165.63 // puro (99.9%)
#define HIDROGENIO 160 // 300 K, 1 atm
#define OURO 127
#define COBRE 111
#define ALUMINIO 97
#define VAPOR_AGUA 23.38
#define FERRO 23
#define AR 19
#define TIJOL 0.52
#define VIDRO 0.34
#define AGUA 0.143 // 25° C
#define BORRACHA 0.1
#define NYLON 0.09
#define MADEIRA 0.082

// Função para calcular o índice da matriz
int pos(int k, int i, int j, int linhas, int colunas){
    return k * (linhas * colunas) + i * colunas + j;
}

// Função para inicializar a matriz
void init(int linhas, int colunas, double* u) {

    // Inicializa toda a matriz com a temperatura ambiente (u_inicial)
    for (int k = 0; k < 2; k++) {
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                u[pos(k, i, j, linhas, colunas)] = u_inicial;
            }
        }
    }

    // Condições de borda
    for (int k = 0; k < 2; k++) {
        for (int i = 0; i < linhas; i++) {
            u[pos(k, i, 0, linhas, colunas)] = u_esq;  // Borda esquerda
        }
        for (int j = 0; j < colunas; j++) {
            u[pos(k, linhas-1, j, linhas, colunas)] = u_baixo;  // Borda inferior
        }
    }
}

// Função para calcular a evolução da matriz ao longo do tempo de forma sequencial
void calcular_seq(int iteracoes, int linhas, int colunas, double* u, double coeficiente) {
    double alpha = coeficiente;
    double delta_x = 1.0;
    double delta_t = (pow(delta_x - 0.05, 2)) / (4 * (alpha + 0.05));  // Pequeno ruído no alfa e no delta x
    double gamma = (alpha * delta_t) / (pow(delta_x, 2));
    
    // Cálculo da evolução ao longo do tempo
    for (int k = 0; k < iteracoes - 1; k++) {
        int cur = k%2;
        int nxt = !cur;
        for (int i = 1; i < linhas - 1; i++) {
            for (int j = 1; j < colunas - 1; j++) {
                // A fórmula para calcular o próximo valor da matriz
                u[pos(nxt, i, j, linhas, colunas)] = gamma *    (u[pos(cur, i+1, j, linhas, colunas)] +
                                                                u[pos(cur, i-1, j, linhas, colunas)] +
                                                                u[pos(cur, i, j+1, linhas, colunas)] +
                                                                u[pos(cur, i, j-1, linhas, colunas)] -
                                                                4 * u[pos(cur, i, j, linhas, colunas)]) +
                                                                u[pos(cur, i, j, linhas, colunas)];
            }
        }
    }
}


// Função principal para gerar a matriz e medir o tempo de execução
double* generate_matrix_seq(int iteracoes, int linhas, int colunas, double coeficiente) {
    clock_t start_time, end_time;
    double total_time;
    double times[10];
   
    // Alocação dinâmica da matriz (tridimensional representada linearmente)
    double *u = (double*) malloc(sizeof(double) * 2 * linhas * colunas);
    if (!u) {
        printf("Erro ao alocar espaço para a matriz.\n");
        exit(1);
    }

    // Inicializando a matriz
    init(linhas, colunas, u);

    // Executando 10 vezes para medir o tempo médio
    for (int i = 0; i < 10; i++) {
        start_time = clock();
       
        // Função para calcular a evolução da matriz
        calcular_seq(iteracoes, linhas, colunas, u, coeficiente);
       
        end_time = clock();

        total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        times[i] = total_time;

        printf("Execution: %d, Total time: %.6f seconds\n", i + 1, total_time);
    }

    // Calculando o tempo médio
    double avg_time = 0.0;
    for (int i = 0; i < 10; i++) {
        avg_time += times[i];
    }
    avg_time /= 10;

    printf("\nAverage execution time: %.6f seconds\n\n", avg_time);
   
    // Abrindo o arquivo para escrita
    FILE *file = fopen("output_matrix_seq.txt", "w");
    if (!file) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        free(u);
        exit(1);
    }

    // Escrevendo os parâmetros da matriz no arquivo
    fprintf(file, "%d ", iteracoes);
    fprintf(file, "%d ", linhas);
    fprintf(file, "%d\n", colunas);

    // Escrevendo os valores da última iteração da matriz
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fprintf(file, "%.6lf ", u[pos((iteracoes-1)%2, i, j, linhas, colunas)]);
        }
        fprintf(file, "\n");
    }

    // Fechando o arquivo
    fclose(file);

    return u;
}

int k; // Variável global para indicar a iteração atual
int contador; // Variável global para auxiliar na barreira

pthread_mutex_t mutex;
pthread_cond_t cond_bar;

typedef struct {
    int linhas, colunas, id, iteracoes, num_threads;
    double gamma, *u;
} t_Args;

void barreira(int num_threads){
    pthread_mutex_lock(&mutex);
    contador++;
    if(contador < num_threads){
        pthread_cond_wait(&cond_bar, &mutex);
    } else {
        contador = 0;
        k++;
        pthread_cond_broadcast(&cond_bar);
    }
    pthread_mutex_unlock(&mutex);
}

void* medir(void* arg){
    t_Args *args = (t_Args*) arg;

    int linhas = args->linhas;
    int colunas = args->colunas;
    int id = args->id;
    int iteracoes = args->iteracoes;
    int num_threads = args->num_threads;

    double gamma = args->gamma;
    double *u = args->u;

    int linhasSemBorda = linhas - 2, colunasSemBorda = colunas - 2; // Desconsiderando as bordas da matriz

    while(k<iteracoes-1){
        
        int cur = k%2;
        int nxt = !cur;

        for(int cont = id; cont<linhasSemBorda*colunasSemBorda; cont+=num_threads){
            int i = cont / colunasSemBorda;
            int j = cont % colunasSemBorda;

            i++, j++; // Acrescento um offset, pois estamos desconsiderando as bordas da matriz

            u[pos(nxt, i, j, linhas, colunas)] = gamma *    (u[pos(cur, i+1, j, linhas, colunas)] +
                                                            u[pos(cur, i-1, j, linhas, colunas)] +
                                                            u[pos(cur, i, j+1, linhas, colunas)] +
                                                            u[pos(cur, i, j-1, linhas, colunas)] -
                                                            4 * u[pos(cur, i, j, linhas, colunas)]) +
                                                            u[pos(cur, i, j, linhas, colunas)];
        }

        barreira(num_threads);
    }

    free(arg);
    pthread_exit(NULL);
}

// Função para calcular a evolução da matriz ao longo do tempo de forma concorrente
void calcular_conc(int iteracoes, int linhas, int colunas, double* u, double coeficiente, int num_threads) {
    double alpha = coeficiente;
    double delta_x = 1.0;
    double delta_t = (pow(delta_x - 0.05, 2)) / (4 * (alpha + 0.05));  // Pequeno ruído no alfa e no delta x
    double gamma = (alpha * delta_t) / (pow(delta_x, 2));

    k = 0;
    contador = 0;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_bar, NULL);

    pthread_t threads[num_threads];

    for(int i=0; i<num_threads; i++){
        t_Args *args;
        args = malloc(sizeof(t_Args));

        if(!args){
            printf("Erro ao alocar espaço para os argumentos.\n");
            exit(1);
        }

        args->linhas = linhas;
        args->colunas = colunas;
        args->id = i;
        args->gamma = gamma;
        args->iteracoes = iteracoes;
        args->num_threads = num_threads;
        args->u = u;

        if(pthread_create(&threads[i], NULL, medir, (void*) args)) {
            printf("Erro ao criar a thread %d\n", i); 
            exit(1);
        }
    }

    for(int i=0; i<num_threads; i++){
        if(pthread_join(threads[i], NULL)){
            printf("Erro ao terminar a thread %d\n", i);
            exit(1);
        }
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_bar);
    
}

double* generate_matrix_conc(int iteracoes, int linhas, int colunas, double coeficiente, int num_threads){
    clock_t start_time, end_time;
    double total_time;
    double times[10];
   
    // Alocação dinâmica da matriz (tridimensional representada linearmente)
    double *u = (double*) malloc(sizeof(double) * 2 * linhas * colunas);
    if (!u) {
        printf("Erro ao alocar espaço para a matriz.\n");
        exit(1);
    }

    // Inicializando a matriz
    init(linhas, colunas, u);

    // Executando 10 vezes para medir o tempo médio
    for (int i = 0; i < 10; i++) {
        start_time = clock();
       
        // Função para calcular a evolução da matriz
        calcular_conc(iteracoes, linhas, colunas, u, coeficiente, num_threads);
       
        end_time = clock();

        total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        times[i] = total_time;

        printf("Execution: %d, Total time: %.6f seconds\n", i + 1, total_time);
    }

    // Calculando o tempo médio
    double avg_time = 0.0;
    for (int i = 0; i < 10; i++) {
        avg_time += times[i];
    }
    avg_time /= 10;

    printf("\nAverage execution time: %.6f seconds\n\n", avg_time);
   
    // Abrindo o arquivo para escrita
    FILE *file = fopen("output_matrix_conc.txt", "w");
    if (!file) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        free(u);
        exit(1);
    }

    // Escrevendo os parâmetros da matriz no arquivo
    fprintf(file, "%d ", iteracoes);
    fprintf(file, "%d ", linhas);
    fprintf(file, "%d\n", colunas);

    // Escrevendo os valores da última iteração da matriz
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fprintf(file, "%lf ", u[pos((iteracoes-1)%2, i, j, linhas, colunas)]);
        }
        fprintf(file, "\n");
    }

    // Fechando o arquivo
    fclose(file);

    return u;
}

// Função para calcular a norma de Frobenius da matriz de diferença entre a sequencial e concorrente
double comp_matrices(double *u1, double *u2, int iteracoes, int linhas, int colunas){
    for(int i=0; i<linhas; i++){
        for(int j=0; j<colunas; j++){
            u1[pos((iteracoes-1)%2, i, j, linhas, colunas)] -= u2[pos((iteracoes-1)%2, i, j, linhas, colunas)];
        }
    }

    double soma = 0;

    for(int i=0; i<linhas; i++){
        for(int j=0; j<colunas; j++){
            soma += u1[pos((iteracoes-1)%2, i, j, linhas, colunas)] * u1[pos((iteracoes-1)%2, i, j, linhas, colunas)];
        }
    }

    return sqrt(soma);
}

int main() {
    int iteracoes = 2000;
    int linhas = 200;
    int colunas = 200;
    int num_threads;
    double *u1, *u2; // matrizes sequencial e concorrente
    
    printf("SEQUENCIAL\n");
    u1 = generate_matrix_seq(iteracoes, linhas, colunas, DIAMANTE);

    for(int i = 1; i<=8; i*=2){
        num_threads = i;
        printf("\nCONCORRENTE (%d threads)\n", num_threads);
        u2 = generate_matrix_conc(iteracoes, linhas, colunas, DIAMANTE, num_threads);
    }

    double diff = comp_matrices(u1, u2, iteracoes, linhas, colunas);

    printf("Norma de Frobenius: %lf", diff); // norma de Frobenius de: (matriz concorrente de 8 threds - matriz sequencial)

    free(u1);
    free(u2);

    return 0;
}