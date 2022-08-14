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

int convertBase(int number,int base){
    if(number == 0 || base==10)
        return number;
    return (number % base) + 10*convertBase(number / base, base);
}

void toBase32(int number, char *numIn32)
{
//    char numString[100];
//    sprintf(numString, "%d", number);
//    int i =0;
//    while(numString[i]!='\0')
//    {
//        char currChar = '\0';
//
//        switch (numString[i]) {
//            case '!': strncat(numIn32, &currChar, 1); break;
//            default: break;
//        }
//
//        i++;
//    }
//    printf("%s\n", numIn32);
}