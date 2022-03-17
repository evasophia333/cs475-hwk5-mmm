#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

int main(int argc, char *argv[])
{
	
	double clockstart, clockend;
	clockstart = rtclock(); // start clocking
	if ((strcmp(argv[1], "P") != 0 && strcmp(argv[1], "S") != 0)|| argc < 3 || argc > 4){
		printf("Usage: ./mmm <mode> [num threads] <size>\n");
		return 0;
	}else if (strcmp(argv[1], "P") == 0 && (atoi(argv[2]) < 1)){
		printf("Error: number of threads should be between 1 and <size>\n");
		return 0;
	}else if (strcmp(argv[1], "S") == 0 && argc == 4){
		printf("Error: sequential mode does not require [num threads]\n");
		return 0;
	}else if (strcmp(argv[1], "P") == 0 && argc == 3){
		printf("Error: parallel mode requires both [num threads] and <size>\n");
		return 0;
	}
	
	if (strcmp(argv[1], "S") == 0){
		int SIZE = atoi(argv[2]);
		mmm_init(SIZE); //initialize
		printf("========\n");
		printf("mode: sequential\n");
		printf("thread count: 1\n");
		printf("size: %d\n", SIZE);

			//run n+1 times the first time
	int sum;
	for (int i = 4; i > 0; i--){
		clockstart = rtclock(); // start clocking
		mmm_seq();
		clockend = rtclock(); // stop clocking
		if (i > 0){
			sum += (clockend - clockstart);
		}
	}
	int sequential = sum/3; //performed three times
	printf("Sequential time: %d sec\n", sequential);
	}
	else{
		int SIZE = atoi(argv[3]);
		mmm_init(SIZE);
		printf("==========\n");
		printf("mode: parallel\n");
		printf("thread count: %d\n", atoi(argv[2]));
		printf("size: %d\n", SIZE);
	}


	// start: stuff I want to clock

	// end: stuff I want to clock

	clockend = rtclock(); // stop clocking
	printf("Time taken: %.6f sec\n", (clockend - clockstart));

	return 0;
}
