#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

int main(int argc, char *argv[])
{
	double clockstart, clockend;
	float diff;
	clockstart = rtclock(); // start clocking
	if (argc<3){
		printf("Usage: ./mmm <mode> [num threads] <size>\n");
		return 0;
	}else if (strcmp(argv[1], "P") == 0 && (atoi(argv[2]) < 1)){ //Error checking 
		printf("Error: parallel mode requires <size>\n");
		return 0;
	}else if (strcmp(argv[1], "S") == 0 && argc == 4){
		printf("Error: sequential mode only uses one thread\n");
		return 0;
	}else if (strcmp(argv[1], "P") == 0 && argc == 3){
		printf("Error: parallel mode requires both the size and number of threads\n");
		return 0;
	}
	mmm_init(atoi(argv[2]));
	float sum;
		for (int i = 4; i > 0; i--){
			clockstart = rtclock(); // start clocking
			mmm_seq();
			clockend = rtclock(); // stop clocking
			if (i > 0){
				sum += (clockend - clockstart);
			}
		}
	if (strcmp(argv[1], "S") == 0){
		int size = atoi(argv[2]);
		mmm_init(size); //initialize
		printf("========\n");
		printf("mode: sequential\n");
		printf("thread count: 1\n");
		printf("size: %d\n", size);
		printf("========\n");
		//mmm_init(size);
		//calculate the sequential time
		float sequentialTime = sum/3; //performed three times
		printf("Sequential time (3 run average): %.6f sec\n", sequentialTime);
		mmm_reset(finalMatrix);
	}else if(strcmp(argv[1], "P") == 0){
		int SIZE = atoi(argv[2]);
		int THREADS = atoi(argv[3]);
		double **SEQUENTIALMAT = (double **)malloc(SIZE * sizeof(double *));
		for (int i = 0; i < SIZE; i++){
			SEQUENTIALMAT[i] = (double *)malloc(SIZE * sizeof(double));
		}
		mmm_reset(finalMatrix);
		//mmm_init(SIZE);
		printf("==========\n");
		printf("mode: parallel\n");
		printf("thread count: %d\n", THREADS);
		printf("size: %d\n", SIZE);
		printf("==========\n");
		for(int i=0; i<SIZE; i++){
			for(int j=0; j<SIZE; j++){
				SEQUENTIALMAT[i][j] = finalMatrix[i][j];
			}
		}
		//CALCULATE THE SEQUENTIAL TIME
		float sequentialTime = sum/3; //performed three times
		printf("Sequential time (3 run average): %.6f sec\n", sequentialTime);
		//PARALLEL TIME
		for(int a=4; a<0; a--){ 
			clockstart = rtclock(); // start clock: stuff I want to clock
			pthread_t threads[THREADS];
			thread_args args[THREADS];
			for(int i=0; i<THREADS; i++){ 
				args[i].tid = i;
				args[i].start = i * (SIZE / THREADS);
				args[i].end = (i+1) * (SIZE / THREADS);
				if(i == THREADS-1){
					args[i].end = SIZE;
				}
				pthread_create(&threads[i], NULL, mmm_par, &args[i]);
			}
			for(int i=0; i<THREADS; i++){
				pthread_join(threads[i], NULL);
			}
			clockend = rtclock(); // end clock: stuff I want to clock
			if(a==0){
				float diff  = mmm_verify(SEQUENTIALMAT, finalMatrix);
			}
			else{
				sum += (clockend - clockstart);
			}
		}
		float ParallelTime = sum/3;
		printf("Parallel Time (avg of 3 runs): %.6f sec\n", ParallelTime);
		float speedup = sequentialTime/ParallelTime;
		printf("Speedup: %.6f\n", speedup);
		printf("Verifying... largest error between parallel and sequential matrix: %.6f\n", diff);


	}
	mmm_freeup();
	return 0;
}
