#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//function to check if a string is a valid float
int is_valid_float(const char *str) {
    char *endptr;
    strtod(str, &endptr);  //attempt to convert string to double
    return (*endptr == '\0');  //if endptr is at the end of the string, it's a valid float
}

int main() {
    FILE *inputFile, *outputFile;
    char buffer[100];
    int invalidCount = 0;
    long fileSize;

    //open input file
    inputFile = fopen("data.txt", "r");
    if (inputFile == NULL) {
        perror("Error opening the input file");
        return 1;
    }

    //get file size
    fseek(inputFile, 0, SEEK_END);  //move file pointer to the end
    fileSize = ftell(inputFile);    //get current position (file size)
    fseek(inputFile, 0, SEEK_SET);  //reset file pointer to the beginning

    //open output file
    outputFile = fopen("valid_floats.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening the output file");
        fclose(inputFile);
        return 1;
    }

    //process file content
    while (ftell(inputFile) < fileSize) {
        if (fscanf(inputFile, "%s", buffer) == 1) {  //read a word from the file
            if (is_valid_float(buffer)) {
                fprintf(outputFile, "%s\n", buffer);  //write valid float to output file
            } else {
                invalidCount++;  //increment invalid count
            }
        } else {
            //skip any whitespace or newline characters
            fseek(inputFile, 1, SEEK_CUR);
        }
    }

    //display results
    printf("Number of invalid values: %d\n", invalidCount);

    //clean up
    if (ferror(inputFile)) {
        perror("Error reading the input file");
    }
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
