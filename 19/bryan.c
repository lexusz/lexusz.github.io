#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

// Function prototypes
int romanToDecimal(char roman[]);
void performOperation(char *line, FILE *outputFile);
void decimalToWords(int number, char *result);
void appendWord(char *result, const char *word);

// Mapping Roman numerals to their decimal values
int valueOfRoman(char ch);

// Main function
int main() {
    FILE *inputFile = fopen("Input.txt", "r");
    FILE *outputFile = fopen("Output.txt", "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    char line[MAX];
    while (fgets(line, sizeof(line), inputFile)) {
        performOperation(line, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);

    printf("Processing completed. Check Output.txt for results.\n");
    return 0;
}

// Convert a Roman numeral to its decimal equivalent
int romanToDecimal(char roman[]) {
    int total = 0, i;
    int length = strlen(roman);

    for (i = 0; i < length; i++) {
        int current = valueOfRoman(roman[i]);
        int next = (i + 1 < length) ? valueOfRoman(roman[i + 1]) : 0;

        if (current < next) {
            total -= current;
        } else {
            total += current;
        }
    }
    return total;
}

// Perform arithmetic operation based on the input line
void performOperation(char *line, FILE *outputFile) {
    char roman1[MAX], roman2[MAX], operation;
    int result;
    char resultWords[MAX] = "";

    // Parse the line
    sscanf(line, "%s %c %s", roman1, &operation, roman2);

    // Convert Roman numerals to decimals
    int num1 = romanToDecimal(roman1);
    int num2 = romanToDecimal(roman2);

    // Perform the operation
    switch (operation) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num2 - num1; // Fix subtraction order
            break;
        case '*':
            result = num1 * num2;
            break;
        default:
            fprintf(outputFile, "Invalid operation\n");
            return;
    }

    // Convert result to words
    decimalToWords(result, resultWords);
    fprintf(outputFile, "%s\n", resultWords);
}

// Get the decimal value of a Roman numeral character
int valueOfRoman(char ch) {
    switch (ch) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0;
    }
}

// Convert decimal number to words
void decimalToWords(int number, char *result) {
    char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *thousands[] = {"", "Thousand", "Million", "Billion"};

    int parts[4] = {0};
    int partCount = 0;

    // Break number into groups of 3 digits
    while (number > 0) {
        parts[partCount++] = number % 1000;
        number /= 1000;
    }

    for (int i = partCount - 1; i >= 0; i--) {
        int part = parts[i];

        if (part >= 100) {
            appendWord(result, ones[part / 100]);
            appendWord(result, "Hundred");
            part %= 100;
        }
        if (part >= 20) {
            appendWord(result, tens[part / 10]);
            part %= 10;
        } else if (part >= 10) {
            appendWord(result, teens[part - 10]);
            part = 0;
        }
        if (part > 0) {
            appendWord(result, ones[part]);
        }
        if (i > 0 && parts[i] > 0) {
            appendWord(result, thousands[i]);
        }
    }

    if (strlen(result) == 0) {
        strcpy(result, "Zero");
    }
}

// Append a word to the result string
void appendWord(char *result, const char *word) {
    if (strlen(result) > 0) {
        strcat(result, " ");
    }
    strcat(result, word);
}
