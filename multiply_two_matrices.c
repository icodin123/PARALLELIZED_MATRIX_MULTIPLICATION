#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#define SIZE 4

void *mult(void *arg)
{
    int *arr = (int *)arg;
    int multiplication_sum = 0, i = 0;

    // array looks like [num_columns, ...columns, ...rows]
    int num_columns = arr[0];

    for (i = 1; i <= num_columns; i++)
    {
        multiplication_sum += arr[i] * arr[i + num_columns]; // sum += current column * current row
    }

    int *p = (int *)malloc(sizeof(int));
    *p = multiplication_sum;

    pthread_exit(p);
}

int main()
{
    int matrix1[SIZE][SIZE];
    int matrix2[SIZE][SIZE];

    // number of rows and columns in matrix 1 and matrix 2 and iteration variables
    int r1 = SIZE, c1 = SIZE, r2 = SIZE, c2 = SIZE, i, j, k;

    for (i = 0; i < r1; i++)
    {
        for (j = 0; j < c1; j++)
        {
            matrix1[i][j] = rand() % 10;
        }
    }

    for (i = 0; i < r1; i++)
    {
        for (j = 0; j < c1; j++)
        {
            matrix2[i][j] = rand() % 10;
        }
    }

    // Displaying matrix A
    for (i = 0; i < r1; i++)
    {
        for (j = 0; j < c1; j++)
        {
            printf("%d ", matrix1[i][j]);
        }
        printf("\n");
    }

    printf("----------------------\n");

    // Displaying matrix B
    for (i = 0; i < r2; i++)
    {
        for (j = 0; j < c2; j++)
        {
            printf("%d ", matrix2[i][j]);
        }
        printf("\n");
    }

    //////////////////// COMPUTATION ////////////////////////////
    int max = r1 * c2;

    pthread_t *threads;
    threads = (pthread_t *)malloc(max * sizeof(pthread_t));
    int count = 0;
    int *data = NULL;

    for (i = 0; i < r1; i++)
    {
        for (j = 0; j < c2; j++)
        {

            // storing row and column elements in data
            data = (int *)malloc((SIZE * 2 + 1) * sizeof(int));

            // first element of the array will be the number of columns
            data[0] = c1;

            for (k = 0; k < c1; k++)
            {
                // copy row i of matrix A into data
                data[k + 1] = matrix1[i][k];
            }

            for (k = 0; k < r2; k++)
            {
                // copy column j of matrix B into data
                data[c1 + k + 1] = matrix2[k][j];
            }

            // create thread to execute multiplication
            pthread_create(&threads[count++], NULL, mult, (void *)(data));
        }
    }

    ////////////////////// PRINT RESULT /////////////////////////

    printf("RESULTANT MATRIX IS:\n");

    for (i = 0; i < max; i++)
    {
        void *k;
        pthread_join(threads[i], &k);
        int *p = (int *)k;
        printf("%d ", *p);

        if ((i + 1) % c2 == 0)
        {
            printf("\n");
        }
    }
    return 0;
}
