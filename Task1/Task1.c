#include <stdio.h>
#include <stdlib.h>

void main() {
    int count = 0;
    float sum_X = 0, sum_Y = 0, sum_XX = 0, sum_YY = 0, sum_XY = 0;
	float a, b;
	float X, Y;
    int x, y,i;
  
    FILE *fptr1, *fptr2, *fptr3, *fptr4;
    char *filenames[] = {"datasetLR1.txt", "datasetLR2.txt", "datasetLR3.txt", "datasetLR4.txt"};

    
    for ( i = 0; i < 4; i++) {
      
        FILE *fptr = fopen(filenames[i], "r");
        if (fptr == NULL) {
            printf("Error opening file: %s\n", filenames[i]);
            exit(1);
        }
      
        while (fscanf(fptr, "%d, %d", &x, &y) != EOF) {
            sum_X += x;
            sum_Y += y;
            sum_XX += x * x;
            sum_XY += x * y;
            count++;
        }
        
        fclose(fptr);
    }

   
    float a_num = ((sum_XX * sum_Y) - (sum_X * sum_XY));
    float a_den = ((count * sum_XX) - (sum_X * sum_X));
    a = a_num / a_den;
    
    float b_num = ((count * sum_XY) - (sum_X * sum_Y));
    float b_den = ((count * sum_XX) - (sum_X * sum_X));
    b = b_num / b_den;
    
   
    printf("y = %f * x + %f\n", b, a);
    
    
    while (1) {
        printf("Enter the value of x (enter a number): \n");
        
      
        if (scanf("%f", &X) == 1) {
            break; 
        } else {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
        }
    }

    
    Y = b * X + a;
    printf("The value of y: %f\n", Y);
}
