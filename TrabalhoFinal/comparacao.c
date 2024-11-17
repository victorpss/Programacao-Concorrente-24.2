#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double calculateFrobeniusNorm(FILE *fPtr1, FILE *fPtr2);

int main()
{
    FILE *fPtr1, *fPtr2;
    double frobeniusNorm;

    fPtr1 = fopen("../output/output_matrix_seq.txt", "r");
    fPtr2 = fopen("../python-files/python_sequencial.txt", "r");

    if (fPtr1 == NULL || fPtr2 == NULL)
    {
        printf("\nErro ao abrir arquivo.\n");
        exit(EXIT_FAILURE);
    }

    printf("SEQUENCIAL\n");

    frobeniusNorm = calculateFrobeniusNorm(fPtr1, fPtr2);
    printf("Norma de Frobenius: %.6f\n", frobeniusNorm);

    fclose(fPtr1);
    fclose(fPtr2);

    fPtr1 = fopen("../output/output_matrix_conc.txt", "r");
    fPtr2 = fopen("../python-files/python_concorrente.txt", "r");

    if (fPtr1 == NULL || fPtr2 == NULL)
    {
        printf("\nErro ao abrir arquivo.\n");
        exit(EXIT_FAILURE);
    }

    printf("\nCONCORRENTE\n");

    frobeniusNorm = calculateFrobeniusNorm(fPtr1, fPtr2);
    printf("Norma de Frobenius: %.6f\n", frobeniusNorm);

    fclose(fPtr1);
    fclose(fPtr2);

    return 0;
}

double calculateFrobeniusNorm(FILE *fPtr1, FILE *fPtr2)
{
    int iterations1, rows1, cols1;
    int iterations2, rows2, cols2;
    double valueA, valueB;
    double sum = 0.0;

    fscanf(fPtr1, "%d %d %d", &iterations1, &rows1, &cols1);
    fscanf(fPtr2, "%d %d %d", &iterations2, &rows2, &cols2);

    if (rows1 != rows2 || cols1 != cols2)
    {
        printf("\nErro: Matrizes possuem dimensões diferentes.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows1; i++)
    {
        for (int j = 0; j < cols1; j++)
        {
            fscanf(fPtr1, "%lf", &valueA);
            fscanf(fPtr2, "%lf", &valueB);

            if(valueA != valueB){
                printf("valor A: %lf, valor B: %lf", valueA, valueB);
            }

            double diff = valueA - valueB;
            sum += diff * diff;
        }
    }

    return sqrt(sum);
}
