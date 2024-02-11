#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

float pi = 0;
float approxPi;

pthread_mutex_t mutex;


struct thread{
	int start;
	int finish;
};

void *threadFun(void *p){
	
	int start;
	int finish;
	int i;
	
	struct thread *targs = (struct thread*)p;
	start = targs->start;
	finish = targs->finish;
	
	pthread_mutex_lock(&mutex);

	for(i = start; i<finish; i++){
		float pi_num = pow(-1, i);
		float pi_den = ((2*i) + 1);
		pi += (pi_num/pi_den);
	}
	
	pthread_mutex_unlock(&mutex);

	pthread_exit(0);
}

int main(){
	int i;
	int threadNum, iterations;
	
	printf("Enter number of iterations(i): ");
	scanf("%d", &iterations);
	
	printf("Enter number of threads: ");
	scanf("%d", &threadNum);

	int chunks[threadNum];
	
	int rem = iterations % threadNum;
	
	struct thread targs[threadNum];
	
	pthread_t thread[threadNum];
	
	pthread_mutex_init(&mutex, NULL);
	
	for(i = 0; i < threadNum; i++){
		chunks[i] = iterations/threadNum;
	}

	for(i = 0; i<rem; i++){
		chunks[i] = chunks[i] + 1;
	}


	for(i = 0; i < threadNum; i++){
		if(i == 0){
			targs[i].start = 0;
		}else{
			targs[i].start = targs[i-1].finish + 1;
		}
		
		targs[i].finish = targs[i].start + chunks[i] - 1;
	}
	
	for(i = 0; i < threadNum; i++){
		pthread_create(&thread[i], NULL, threadFun, &targs[i]);
	}
	
	for(i = 0; i < threadNum; i++){
		pthread_join(thread[i], NULL);
	}
	
	pthread_mutex_destroy(&mutex);
	
	approxPi = pi * 4;
	printf("The appropiate value of PI is %f", approxPi);
}
