#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Declaring file pointers for five files
FILE *fptr1, *fptr2, *fptr3, *fptr4, *fptr5;

// Global variable to store the count of prime numbers
int count = 0;

// Structure to store the start and finish index of the range to be processed by each thread
struct thread{
	int start;
	int finish;
} args;

// Function to check if the number is prime or not
void primeFunction(int primeNum){
	int i;
	int boolean = 0;
	
	// If the number is less than 2, it is not a prime number
	if(primeNum < 2){
		boolean = 1;
	}

	// Loop through all the numbers less than the given number
	for(i = 2; i < primeNum; i++){
		// If the number is divisible by any number in the range, it is not a prime number
		if(primeNum % i == 0){
			boolean = 1;
			break;
		}
	}

	// If the number is not divisible by any number in the range, it is a prime number
	if(boolean == 0){
		// Writing the prime number to the file pointed by fptr5
		fprintf(fptr5, "%d\n", primeNum);
		// Incrementing the count of prime numbers
		count++;
	}
	
}

// Thread function that will be called by pthread_create function
void *threadFunction(void *p){
	int start;
	int finish;
	int i, num;
	
	// Casting the void pointer to struct thread pointer
	struct thread *targs = (struct thread*)p;

	// Assigning the start and finish values from the structure to local variables
	start = targs->start;
	finish = targs->finish;

	// Loop through the range assigned to the thread
	for(i = start; i<finish; i++){
		// Reading a number from the file pointed by fptr4
		fscanf(fptr4, "%d", &num);
		// Checking if the number is prime or not
		primeFunction(num);
	
	}
	// Exiting the thread
	pthread_exit(0);
}

int main(){
	int num, i;
	int row = 0;
	int threadNum;
	
	// Taking the number of threads as input from the user
	printf("Enter the number of threads: ");
	scanf("%d", &threadNum);
	
	// Array to store the range of numbers assigned to each thread
	int chunks[threadNum];
	
	// Allocating memory for the structure to store the range of numbers
	struct thread *targs = malloc(threadNum * sizeof(struct thread));

	// Array to store the thread IDs
	pthread_t thread[threadNum];

	// Opening the  file in read mode
	fptr1 = fopen("PrimeData1.txt", "r");
	fptr2 = fopen("PrimeData2.txt", "r");
	fptr3 = fopen("PrimeData3.txt", "r");
	// Opening the file in write mode
	fptr4 = fopen("AllNumbers.txt", "w");

	//reading from file pointer
	while(fscanf(fptr1, "%d", &num) != EOF){
		row++;
	}
	while(fscanf(fptr2, "%d", &num) != EOF){
		row++;
	}
	while(fscanf(fptr3, "%d", &num) != EOF){
		row++;
	}

	int rem = row % threadNum;

	fclose(fptr1);
	fclose(fptr2);
	fclose(fptr3);

	fptr1 = fopen("PrimeData1.txt", "r");
	fptr2 = fopen("PrimeData2.txt", "r");
	fptr3 = fopen("PrimeData3.txt", "r");
	fptr5 = fopen("PrimeNumbers.txt", "w");

	//Slicing to each threads
	for(i = 0; i < threadNum; i++){
		chunks[i] = row / threadNum;
	}

	for(i = 0; i < rem; i++){
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
	
	//Reading from FILE pointer and printing on new pointer file
	while(fscanf(fptr1, "%d", &num) != EOF){
		fprintf(fptr4, "%d\n", num);
	}
	while(fscanf(fptr2, "%d", &num) != EOF){
		fprintf(fptr4, "%d\n", num);
	}
	while(fscanf(fptr3, "%d", &num) != EOF){
		fprintf(fptr4, "%d\n", num);
	}
	
	fclose(fptr1);
	fclose(fptr2);
	fclose(fptr3);
	
	fptr4 = fopen("AllNumbers.txt", "r");
	
	for(i = 0; i < threadNum; i++){
		pthread_create(&thread[i], NULL, threadFunction, &targs[i]);
	}
	for(i = 0; i < threadNum; i++){
		pthread_join(thread[i], NULL);
	}
	
	fprintf(fptr5, "The total Prime numbers are:\n %d\n", count);
	printf("The total Prime numbers are:\n %d\n", count);
	
	fclose(fptr4);
	fclose(fptr5);
	
	free(targs);
	return 0;

	
}


