#include <stdio.h>
#include <stdlib.h>

/* Declare your functions here */
int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);

/* Array of function pointers */
int (*operations[4])(int, int) = {add, subtract, multiply, divide};

int main(void)
{
    int a = 6, b = 3;
    char input;

    printf("Enter a single character ('0' for add, '1' for subtract, '2' for multiply, '3' for divide, '4' to exit): ");
    input = getchar();

    /* Check if input is '4' to exit */
    if (input == '4') {
        printf("Exiting program.\n");
        return 0;
    }

    /* Perform the operation based on user input */
    int index = input - '0';
    if (index >= 0 && index < 4) {
        int result = operations[index](a, b);
        printf("Result: %d\n", result);
    } else {
        printf("Invalid input.\n");
    }

    return 0;
}

/* Define your functions here */
int add(int a, int b) { printf("Adding 'a' and 'b'\n"); return a + b; }
int subtract(int a, int b) { printf("Subtracting 'b' from 'a'\n"); return a - b; }
int multiply(int a, int b) { printf("Multiplying 'a' and 'b'\n"); return a * b; }
int divide(int a, int b) { 
    if (b != 0) {
        printf("Dividing 'a' by 'b'\n"); 
        return a / b; 
    } else {
        printf("Division by zero error\n");
        return 0;
    }
}