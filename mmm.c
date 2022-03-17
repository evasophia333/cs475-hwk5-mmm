#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "mmm.h"

int globalsize;
double** firstMatrix;
double** secondMatrix;
double** finalMatrix;

/**
 * Allocate and initialize the matrices on the heap. Populate
 * the input matrices with random integers from 0 to 99
 */
void mmm_init(int size)
{
	globalsize = size;
	//part one of memory allocation
	firstMatrix = (double**)malloc(size * sizeof(double *)); 
	secondMatrix = (double**)malloc(size * sizeof(double *)); 
	finalMatrix = (double**)malloc(size * sizeof(double *)); 
	//them allocate within the allocation
	for (int i = 0; i < size; i++){ 
        firstMatrix[i] = (double *)malloc(size * sizeof(double));
		secondMatrix[i] = (double *)malloc(size * sizeof(double));
		finalMatrix[i] = (double *)malloc(size * sizeof(double));
    }
	//initialize with random integers within the matricies between 0 and 99
	for (int i = 0; i < size; i++){ 
        for (int j = 0; j < size; j++){
            firstMatrix[i][j] = rand() % 100;
			secondMatrix[i][j] = rand() % 100;
        }
    }
}

/**
 * Reset a given matrix to zeroes
 * @param matrix pointer to a 2D array
 */
void mmm_reset(double **matrix)
{
	for(int i=0; i<globalsize; i++){
		for(int j=0; j<globalsize; j++){
			matrix[i][j]= 0;
		}
	}
}

/**
 * Free up memory allocated to all matrices
*/
void mmm_freeup()
{
	//free the memory of each of the matricies 
	free(firstMatrix);
    free(secondMatrix);
    free(finalMatrix);
	for (int i = 0; i < globalsize; i++){
		free(finalMatrix[i]);
        free(firstMatrix[i]);
		free(secondMatrix[i]);
    }
}

/**
 * Sequential MMM
*/
void mmm_seq()
{
	int total; //local var that keeps the value and adds it to the final matrix
	for (int i = 0; i < globalsize; i++){
        for (int j = 0; j < globalsize; j++){
			for (int k = 0; k < globalsize; k++){
				total += firstMatrix[i][k] * secondMatrix[k][j];
			}
			finalMatrix[i][j] = total;
			total = 0;
        }
    }
}

/**
 * Parallel MMM
 */
void *mmm_par(void *args)
{
  	thread_args *thread = (thread_args *)args;
	double sum;
	for (int i = thread->begin; i <= thread->end; i++){
        for (int j = 0; j < globalsize; j++){
			for (int a = 0; a < globalsize; a++){
				sum += firstMatrix[i][a] * secondMatrix[a][j];
			}
			finalMatrix[i][j] = sum;
			sum = 0;
        }
    }
}

/**
 * Verifies the correctness between the matrices generated by
 * the sequential run and the parallel run.
 *
 * @return the largest error between two corresponding elements
 * in the result matrices
*/
double mmm_verify()
{
	int difference; 
	for (int i = 0; i < globalsize; i++){
        for (int j = 0; j < globalsize; j++){
            if (fabs(firstMatrix[i][j] - secondMatrix[i][j]) > difference){
				difference = fabs(firstMatrix[i][j] - secondMatrix[i][j]);
			}
        }
    }
	return difference;
}  
 
  