
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

double **A, **B, **C;
int number_of_threads = 1;

int* matrix_part_size;
int** matrix_part;
int rowsA, rowsB, columnsA, columnsB;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
double matrix_norm;

void *ThreadFunction(void* arg)
{
	 int i, x;
	 int thread_id = *((int*)arg);

	 int current_matrix_part_size;
	 int row, column;
	 double vectorA[columnsA];
	 double vectorB[rowsB];
	 double sum = 0;
	

	 pthread_mutex_lock(&mutex);
     current_matrix_part_size = (int)matrix_part_size[thread_id];
	 pthread_mutex_unlock(&mutex);
	
	for (x = 0; x < current_matrix_part_size; x++)
	{
		sum = 0;
		pthread_mutex_lock(&mutex);
	    row = matrix_part[thread_id][x*2];	
	    column = matrix_part[thread_id][(x*2)+1];	

		for (i = 0; i < columnsA; i++)
		{
			vectorA[i] = A[row][i];
			vectorB[i] = B[i][column];


		}
		
		pthread_mutex_unlock(&mutex);

		for (i = 0; i < columnsA; i++)
		{
			sum += vectorA[i] * vectorB[i];
		}

		pthread_mutex_lock(&mutex);
		C[row][column] = sum;
		matrix_norm += sum*sum;
		pthread_mutex_unlock(&mutex);
	}
	
	free(arg);
}

void MultiplyMatrix(double**A, int a, int b, double**B, int c, int d, double**C)
{
    int i, j, k;
	int current_thread = 0;
    

    pthread_t threads[number_of_threads];
	
    for(i=0; i<number_of_threads; i++)
    {
		int *thread_function_arg = malloc(sizeof(int));       
		*thread_function_arg = current_thread;


		if (pthread_create(&(threads[current_thread]), NULL, ThreadFunction, thread_function_arg) != 0)
        {
            printf("Thread creation error.");
            abort();
        }
		else{
			printf("Thread created - nr %d\n",current_thread);
		}
		current_thread++;
	}
	
	for (i = 0; i < number_of_threads; i++)
    {

        if (pthread_join(threads[i], NULL))
        {
            printf("Error with threads ending.");
            abort();
        }
    }

}

PrintMatrix(double**A, int m, int n)
{
    int i, j;
    printf("[");
    for(i =0; i< m; i++)
    {
        for(j=0; j<n; j++)
        {
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }
    printf("]\n");
}

int main(int argc, char *argv[])
{
    FILE *fileA = fopen("A.txt", "r");
    FILE *fileB = fopen("B.txt", "r");
    int i, j, k;
    double x;
	number_of_threads = atoi(argv[1]);

    if( fileA == NULL || fileB == NULL )
    {
        printf("File Error");
        exit(-10);
    }

    fscanf (fileA, "%d", &rowsA);
    fscanf (fileA, "%d", &columnsA);
    fscanf (fileB, "%d", &rowsB);
    fscanf (fileB, "%d", &columnsB);
	
	if(columnsA != rowsB)
    {
        printf("Bad Matrix Size!\n");
        return EXIT_FAILURE;
    }

    printf("1st Matrix: %d x %d\n 2nd Matrix: %d x %d\n", rowsA, columnsA, rowsB, columnsB);
	printf("1st Solved Matrix size %d x %d\n\n", rowsA, columnsB);
		
	int cells_count = rowsA * columnsB;
	int cells_per_thread = cells_count / number_of_threads;
	int cells_remainder = cells_count - (cells_per_thread * number_of_threads); 
	
	printf("Cells Number %d\n", cells_count);
	printf("Threads number %d\n", number_of_threads);
	printf("Average cells number per 1 thread %d ( + %d bonus )\n\n", cells_per_thread, cells_remainder);
	
	matrix_part_size = malloc(sizeof(int) * number_of_threads); 
	matrix_part = malloc(sizeof (int*) * number_of_threads); 

	for (i = 0; i < number_of_threads; i++)
	{
		if (i == 0)
		{
			matrix_part[i] = malloc(sizeof(int) * (cells_per_thread + cells_remainder) * 2);
			matrix_part_size[i] = (cells_per_thread + cells_remainder);
		}
		else
		{
			matrix_part[i] = malloc(sizeof(int) * cells_per_thread * 2);
			matrix_part_size[i] = cells_per_thread;
		}
	
	}
	
	j = 0;
	for (i = 0; i < number_of_threads; i++)
	{
		for (k = 0; k < matrix_part_size[i]; k++)
		{
			matrix_part[i][k*2] = j / columnsB; 
			matrix_part[i][(k*2)+1] = j % columnsB; 


			j++;

		}
	}
	
    A = malloc(rowsA*sizeof(double));
    for(i=0; i< rowsA; i++) A[i] = malloc(columnsA*sizeof(double));
    
    B = malloc(rowsB*sizeof(double));
    for(i=0; i< rowsB; i++) B[i] = malloc(columnsB*sizeof(double));
    
    C = malloc(rowsA*sizeof(double));
    for(i=0; i< rowsA; i++) C[i] = malloc(columnsB*sizeof(double));
    
    for(i =0; i< rowsA; i++)
    {
        for(j = 0; j<columnsA; j++)
        {
            fscanf( fileA, "%lf", &x );
            A[i][j] = x;
        }
    }

    printf("Matrix A:\n");
    PrintMatrix(A, rowsA, rowsB);

    for(i =0; i< rowsB; i++)
    {
        for(j = 0; j<columnsB; j++)
        {
            fscanf( fileB, "%lf", &x );
            B[i][j] = x;
        }
    }

    printf("Matrix B:\n");
    PrintMatrix(B, rowsB, columnsB);

    MultiplyMatrix(A, rowsA, columnsA, B, rowsB, columnsB, C);

	printf("\n Result:\n");
	printf("Frobenius Equation: %f\n", sqrt(matrix_norm));

    printf("Matrix C:\n");
    PrintMatrix(C, rowsA, columnsB);

    for(i=0; i<columnsA; i++) free(A[i]);
    free(A);

    for(i=0; i<columnsB; i++) free(B[i]);
    free(B);

    for(i=0; i<columnsB; i++) free(C[i]);
    free(C);
	
	for(i=0; i< number_of_threads; i++) free(matrix_part[i]);
    free(matrix_part);
	
    free(matrix_part_size);
	
    fclose(fileA);
    fclose(fileB);

    return 0;
}