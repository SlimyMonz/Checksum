/*=============================================================================
 |   Purpose - Calculating an 8, 16, or 32 bit
 |             checksum on an ASCII input file
 |
 |   Author: Harry James Hocker
 |   Created: November 12th, 2021
 |   Language: C
 |
 |   To Compile:  gcc -o checksum checksum.c
 |
 |   To Execute:  ./checksum inputFile.txt X
 |   inputFile.txt is an 8-bit ASCII input file
 |   X can be: 8, 16, or 32 which are the valid checksum sizes
 |   all other values are rejected with an error message
 |
 +=============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// take file and return the array form of the file
char* fileToString(FILE* source)
{
    int index = 0;
    char c; char* destination = calloc(1, sizeof(char));
    // while the file has content add to array
    while ((c=fgetc(source)) != EOF)
    {
        // dynamically increase size of character array
        destination = realloc(destination, (index+1)*sizeof(char));
        // add character to each index
        destination[index] = c;
        // increment index each loop
        index++;
    }
    return destination;
}

char* padding_16(char* string)
{
    unsigned long length = strlen(string);
    unsigned long int padAmount = (2 - (length % 2))%2;
    
    if (padAmount) {
        string = realloc(string, (length+padAmount+1)*(sizeof(char)));
        for (int i = 0; i < padAmount; i++) {
            string[length+i] = 'X';
            printf("X");
        }
    }
    string[length+padAmount] = '\0';
    printf("\n");
    return string;
}

char* padding_32(char* string)
{
    unsigned long length = strlen(string);
    unsigned long int padAmount = (4 - (length % 4))%4;
    
    if (padAmount) {
        string = realloc(string, (length+padAmount+1)*(sizeof(char)));
        for (int i = 0; i < padAmount; i++) {
            string[length+i] = 'X';
            printf("X");
        }
    }
    string[length+padAmount] = '\0';
    printf("\n");
    return string;
}


// prints out a string with 80 characters per line
void printString(char* string)
{
    printf("\n");
    unsigned long length = strlen(string);
    for (int i = 0; i < length; i++) {
        if ((i != 0) && (i % 80 == 0)) printf("\n");
        printf("%c", string[i]);
    }
}

unsigned long checksum_8(char* string, unsigned long long length)
{
    unsigned long result = 0;
    
    for (int i = 0; i < length; i++)
    {
        result += string[i];
    }
    return result & 0xff;
}

unsigned long checksum_16(char* string, unsigned long long length)
{
    unsigned long result = 0;
    
    for (int i = 0; i < length; i+=2)
    {
        result += (string[i] << 8);
        result += (string[i+1]);
    }
    return result & 0xffff;
}

unsigned long checksum_32(char* string, unsigned long long length)
{
    unsigned long result = 0;
    
    for (int i = 0; i < length; i+=4)
    {
        result += (string[i] << 24);
        result += (string[i+1] << 16);
        result += (string[i+2] << 8);
        result += (string[i+3]);
    }
    return result & 0xffffffff;
}

int errorCheck(int argc, const char* argv[])
{
    // Wrong number of arguments passed
    if (argc != 3)
    {
        fprintf(stderr, "Invalid input: Not enough arguments.\n");
        return -1;
    }
    
    if (fopen(argv[1], "r") == NULL) {
        fprintf(stderr, "Invalid filename.\n");
        return -1;
    }
    
    int bits = atoi(argv[2]);
    if (!(bits == 8 || bits == 16 || bits == 32))
    {
        fprintf(stderr, "Invalid checksum size. Must be 8, 16, or 32.\n");
        return -1;
    }
    // no issues, return 0.
    return 0;
}


int main(int argc, const char* argv[])
{
    // make sure the inputs are good
    if (errorCheck(argc, argv) == -1) return -1;
    
    // Open the input file
    FILE* file = fopen(argv[1], "r");

    // get checksum bit size
    int bits = atoi(argv[2]);

    // put the file into a string
    char* string = fileToString(file);
    // print out the string
    printString(string);
    // close the file
    fclose(file);
    
    unsigned long length = 0;
    unsigned long checksum = 0;
    
    switch(bits) {
        case 8 :
            length = strlen(string);
            checksum = checksum_8(string, length);
            break;
        case 16 :
            string = padding_16(string);
            length = strlen(string);
            checksum = checksum_16(string, length);
            break;
        case 32 :
            string = padding_32(string);
            length = strlen(string);
            checksum = checksum_32(string, length);
            break;
    }
    
    // print everything out into a single line as-formatted
    printf("%2d bit checksum is %8lx for all %4lu chars\n", bits, checksum, length);
    // prevent a memory issue
    free(string);
    
    return 0;
}
