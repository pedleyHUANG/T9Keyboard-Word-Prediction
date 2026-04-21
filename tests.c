#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "t9_lib.h"
#include "t9_priv.h"

/* This is a test main to break down individual tests from t9_test.c
*/

// Compliling input: gcc -g -Wall -std=c11 -o tests tests.c

// compile: gcc -g -Wall -std=c11 -o tests tests.c t9_lib.c

// valgrind --leak-check=full ./myprogram

/*
printing statement that could be inserted
printf(">>");
%d for integer
%c for character
%s for string
%f for float
*/

int char2Num(char letter);

int main(int argc, char* argv[]) {
    T9* dict = InitializeEmptyT9();
    T9* dict1 = InitializeEmptyT9();
    AddWordToT9(dict, "abcdefghijklmnopqrstuvwxyz");
    char* lowerCase = PredictT9(dict, "22233344455566677778889999");
    AddWordToT9(dict1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    char* upper = PredictT9(dict1, "22233344455566677778889999");
    DestroyT9(dict);
    DestroyT9(dict1);
    return EXIT_SUCCESS;
}
