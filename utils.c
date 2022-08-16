//
// Created by dekke on 07/08/2022.
//
#include "utils.h"

void readFirstWordInLine(char line[], char *word)
{
    int lineIndex = 0;
    int wordIndex = 0;

    while(isspace(line[lineIndex]) && line[lineIndex] != '\0')
    {
        lineIndex++;
    }

    while(!isspace(line[lineIndex]) && line[lineIndex] != '\0')
    {
        word[wordIndex] = line[lineIndex];
        lineIndex++;
        wordIndex++;
    }
}

int stringToInt(char *number)
{
    char p = *number;
    int intNumber = 0;
    int sign = 1;

    if ( '-' == *number || '+' == *number) {
        if ( '-' == *number) {
            sign = -1;
        }
        number++;
    }

    while (*number) {
        p = *number - '0';
        if ( 0 <= p && 9 >= p) {// digit 0 to 9
            intNumber = intNumber * 10 + p;
            number++;
        }
        else {
            break;//not a digit
            // TODO: error raise - not a digit where should be
        }
    }

    return intNumber * sign;
}

char charValue(int num)
{
    switch(num){
        case 0: return '!';
        case 1: return '@';
        case 2: return '#';
        case 3: return '$';
        case 4: return '%';
        case 5: return '^';
        case 6: return '&';
        case 7: return '*';
        case 8: return '<';
        case 9: return '>';
        case 10: return 'a';
        case 11: return 'b';
        case 12: return 'c';
        case 13: return 'd';
        case 14: return 'e';
        case 15: return 'f';
        case 16: return 'g';
        case 17: return 'h';
        case 18: return 'i';
        case 19: return 'j';
        case 20: return 'k';
        case 21: return 'l';
        case 22: return 'm';
        case 23: return 'n';
        case 24: return 'o';
        case 25: return 'p';
        case 26: return 'q';
        case 27: return 'r';
        case 28: return 's';
        case 29: return 't';
        case 30: return 'u';
        case 31: return 'v';
        default: break;
    }
}

// Utility function to reverse a string
void reverseStr(char *str)
{
    int len = strlen(str);
    int i;
    for (i = 0; i < len/2; i++)
    {
        char temp = str[i];
        str[i] = str[len-i-1];
        str[len-i-1] = temp;
    }
}

// Function to convert a given decimal number to base 32 of 10 bit width
char* decTo32(char res[], short inputNum)
{
    int index = 0;  // Initialize index of result
    short base = 32;

    // Convert input number is given base by repeatedly
    // dividing it by base and taking remainder
    while (inputNum > 0)
    {
        res[index++] = charValue(inputNum % base);
        inputNum /= base;
    }
    if(index == 0)
    {
        res[index++] = '!';
    }
    if(index == 1)
    {
        res[index++] = '!';
    }
    res[index] = '\0';

    // Reverse the result
    reverseStr(res);

    return res;
}