#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<pthread.h>

int Prime = 0;  // Global variable to store the count of prime numbers

// Structure to hold input data for each thread
typedef struct {
    int initialPosition;
    int finalPosition;
    FILE *fp;
    int *arr;
} Input;

pthread_mutex_t mx;  // Mutex for thread synchronization

// Function to check if a number is prime
bool isPrime(int value) {
    int i;
    for (i = 2; i < (value / 2); i++) {
        if (value % i == 0) {
            return false;
        }
    }
    return true;
}

// Thread function to find prime numbers in a given range
void *POSIX(void *p) {
    Input *mark = (Input *)p;
    FILE *file = mark->fp;
    int i;
    bool test;

    pthread_mutex_lock(&mx);  // Lock mutex to avoid data race

    // Iterate over the specified range and check for prime numbers
    for (i = mark->initialPosition; i <= mark->finalPosition; i++) {
        test = isPrime(mark->arr[i]);
        if (test == true) {
            fprintf(file, "%d\n", mark->arr[i]);  // Write prime number to file
            Prime += 1;
        }
    }

    pthread_mutex_unlock(&mx);  // Unlock mutex after finishing the calculation

    pthread_exit(0);
}

// Main function
int main() {

    FILE *fp1, *fp2, *fp3, *fp4;
    int totalNumber = 0;
    int n, a, i, x;

    // Opening files in read mode
    fp1 = fopen("PrimeData1.txt", "r");
    fp2 = fopen("PrimeData2.txt", "r");
    fp3 = fopen("PrimeData3.txt", "r");

    // Check if files are opened successfully
    if (fp1 == NULL || fp2 == NULL || fp3 == NULL) {
        printf("Failed to open file!\n");
        return 0;
    }

    // Counting the total number of lines in the files
    while ((a = fgetc(fp1)) != EOF) {
        if (a == '\n') {
            totalNumber++;
        }
    }

    while ((a = fgetc(fp2)) != EOF) {
        if (a == '\n') {
            totalNumber++;
        }
    }

    while ((a = fgetc(fp3)) != EOF) {
        if (a == '\n') {
            totalNumber++;
        }
    }

    // Setting file pointers to the beginning of the files
    fseek(fp1, 0, SEEK_SET);
    fseek(fp2, 0, SEEK_SET);
    fseek(fp3, 0, SEEK_SET);

    // Creating a temporary array to store numbers from the files
    int *temp = malloc(totalNumber * sizeof(int));
    i = 0;

    // Reading numbers from the first file and storing them in the array
    while (fscanf(fp1, "%d", &x) != EOF) {
        temp[i] = x;
        i = i + 1;
    }

    // Reading numbers from the second file and storing them in the array
    while (fscanf(fp2, "%d", &x) != EOF) {
        temp[i] = x;
        i = i + 1;
    }

    // Reading numbers from the third file and storing them in the array
    while (fscanf(fp3, "%d", &x) != EOF) {
        temp[i] = x;
        i = i + 1;
    }

    // Printing the total number of numbers in the files
    printf("Total Count = %d\n", totalNumber);

    // Asking the user to input the number of threads
    printf("Enter the thread number: ");
    scanf("%d", &n);

    // Creating an array to store the number of elements in each thread
    int *list = malloc(n * sizeof(int));
    for (i = 0; i < n; i++) {
        list[i] = (totalNumber / n);
    }

    // Adding the remaining elements to the first few threads
    for (i = 0; i < (totalNumber % n); i++) {
        list[i] = list[i] + 1;
    }

    // Creating an array to store the starting and ending positions of each thread
    int *list_start = malloc(n * sizeof(int));
    int *list_end = malloc(n * sizeof(int));

    // Calculating the starting and ending positions of each thread
    for (i = 0; i < n; i++) {
        if (i == 0) {
            list_start[i] = 0;
        } else {
            list_start[i] = list_end[i - 1] + 1;
        }
        list_end[i] = list_start[i] + list[i] - 1;
    }

    // Opening a file to write the prime numbers
    fp4 = fopen("PrimeNumbers.txt", "w");

    // Creating an array of structures to store the starting and ending positions of each thread
    Input *th_pos = malloc(n * sizeof(Input));
    for (i = 0; i < n; i++) {
        th_pos[i].initialPosition = list_start[i];
        th_pos[i].finalPosition = list_end[i];
        th_pos[i].fp = fp4;
        th_pos[i].arr = temp;
    }

    pthread_mutex_init(&mx, NULL);  // Initialize mutex for synchronization

    // Creating an array of threads
    pthread_t *th = malloc(n * sizeof(pthread_t));

    // Creating the threads
    for (i = 0; i < n; i++) {
        pthread_create(&th[i], NULL, POSIX, &th_pos[i]);
    }

    // Waiting for all threads to finish
    for (i = 0; i < n; i++) {
        pthread_join(th[i], NULL);
    }

    pthread_mutex_destroy(&mx);  // Destroy mutex after usage

    // Printing the total count of prime numbers
    printf("Total prime numbers: %d\n", Prime);

    // Freeing allocated memory
    free(list);
    free(list_start);
    free(list_end);
    free(th_pos);
    free(th);
    free(temp);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);

    return 0;
}
