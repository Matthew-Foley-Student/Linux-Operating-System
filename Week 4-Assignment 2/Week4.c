#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void print_binary(int number) {
    int i;
    for (i = 15; i >= 0; i--) {
        printf("%d", (number >> i) & 1);
    }
    printf("\n\n");
}



int main() {
    int pageSize;
    int userNumber;

    // Prompt user for a valid page size
    printf("Enter page size (4095 or 8191): ");
    if (scanf("%d", &pageSize) != 1 || (pageSize != 4095 && pageSize != 8191)) {
        printf("Invalid page size entered. Exiting.\n\n");
        return 1;
    }

    //Display the page size in decimal, binary, and hexadecimal
    printf("\n\n--- Page Size Information ---\n\n");
    printf("Decimal: %d\n\n", pageSize);
    printf("Binary (16-bit): ");
    print_binary(pageSize);
    printf("Hexadecimal: 0x%04X\n\n", pageSize);

    //Prompt the user for a number to convert
    printf("\n\nEnter a decimal number to convert: ");
    if (scanf("%d", &userNumber) != 1) {
        printf("Invalid number entered. Exiting.\n\n");
        return 1;
    }
    printf("\n\n--- User Number Information ---\n\n");
    printf("Decimal: %d\n\n", userNumber);
    printf("Binary (16-bit): ");
    print_binary(userNumber);
    printf("Hexadecimal: 0x%04X\n\n", userNumber);
    
    return 0;
}
