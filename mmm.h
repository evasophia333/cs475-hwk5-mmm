#ifndef MMM_H_
#define MMM_H_
//FUNCTIONS
void mmm_init(int size);
void mmm_reset(double **);
void mmm_freeup();
void mmm_seq();
void *mmm_par(void *args);
double mmm_verify();

//VARIABLES
extern int globalsize; 
extern int **firstMatrix;
extern int **secondMatrix;
extern int **finalMatrix;

//holds info for the parallel MMM
typedef struct thread_args
{
  int tid;    //Thread ID
  int start; 
  int end; 
} thread_args;

#endif /* MMM_H_ */
