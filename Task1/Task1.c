#include <stdio.h>
#include <stdlib.h>

// Function to calculate linear regression coefficients
void main() {
    int count = 0;
    float sum_X = 0, sum_Y = 0, sum_XX = 0, sum_YY = 0, sum_XY = 0;
    float a, b; // Coefficients for linear regression
    float X, Y; // Input and output variables
    int x, y, i; // Temporary variables and loop counter

    // Array of filenames for datasets
    char *filenames[] = {"datasetLR1.txt", "datasetLR2.txt", "datasetLR3.txt", "datasetLR4.txt"};

    // Loop through each dataset file
    for (i = 0; i < 4; i++) {
        FILE *fptr = fopen(filenames[i], "r");
        if (fptr == NULL) {
            printf("Error opening file: %s\n", filenames[i]);
            exit(1);
        }

        // Read data from the file
        while (fscanf(fptr, "%d, %d", &x, &y) != EOF) {
            sum_X += x;
            sum_Y += y;
            sum_XX += x * x;
            sum_XY += x * y;
            count++;
        }

        // Close the file
        fclose(fptr);
    }

    // Calculate linear regression coefficients (a and b)
    float a_num = ((sum_XX * sum_Y) - (sum_X * sum_XY));
    float a_den = ((count * sum_XX) - (sum_X * sum_X));
    a = a_num / a_den;

    float b_num = ((count * sum_XY) - (sum_X * sum_Y));
    float b_den = ((count * sum_XX) - (sum_X * sum_X));
    b = b_num / b_den;

    // Print the linear regression equation
    printf("y = %f * x + %f\n", b, a);

    // User input for prediction
    while (1) {
        printf("Enter the value of x (enter a number): \n");

        // Validate user input
        if (scanf("%f", &X) == 1) {
            break; // Exit loop on valid input
        } else {
            while (getchar() != '\n'); // Clear input buffer on invalid input
            printf("Invalid input. Please enter a number.\n");
        }
    }

    // Calculate and print the predicted value of y
    Y = b * X + a;
    printf("The predicted value of y: %f\n", Y);
}
