/**

    Author: Pedley Jiaxin Huang
    UWNetID: jh343
    CSE 374 25su Homework 6: T9 Implementation
    Date: 8/11/2025

    Citations:
    on File reading (line by line)
    https://www.tutorialkart.com/c-programming/how-to-read-a-file-line-by-line-in-c/
    writing Trie
    https://en.wikipedia.org/wiki/Trie
    C traversal of treelike data structures
    https://www.tutorialspoint.com/data_structures_algorithms/tree_traversal_in_c.htm

    Description: This program implements the functions of a T9 prediction. Users types
    in numbers combination between 2 and 9, the possible English word of the number
    combination will be predicted base on a dictionary. The users can add "#"s to the 
    end of the numbers to shift through the prediction.
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "t9_lib.h"
#include "t9_priv.h"

#define WORD_LOCATION 8

// Base on the character, return the number of the characters stored in T9
int char2Num(char ch);

// Creates a T9 node based on the number representing T9 characters
T9* makeT9(int number, const char* word);

// Add a whole word to the part of a T9 node where it represents end of a word
void makeWord(T9* node, const char* word);

// return a new string that removes the trailing new line (if any)
// from the line read buffer
char* trimWord(char line[]);

T9* InitializeEmptyT9() {
    T9* root = makeT9(1, NULL);
    return root;
}

T9* InitializeFromFileT9(const char* filename) {
    T9* root = InitializeEmptyT9();
    FILE* fptr = fopen(filename, "r");
    // Handle non-existing file
    if (fptr == NULL) {
        fprintf(stderr, "Fail to read file.\n");
        return NULL;
    }
    char line[51];
    while (fgets(line, sizeof(line), fptr) != NULL) {
        char* word = trimWord(line);
        AddWordToT9(root, word);
        free(word);
    }
    fclose(fptr);
    return root;
}

// Add a natural C string into the Trie
void AddWordToT9(T9* dict, const char* word) {
    if (word == NULL) {
        return;
    }
    T9* currNode = dict;
    for (int i = 0; word[i] != '\0'; i++) {
        // preparation of interpreting each character
        char curr = word[i];
        int num = char2Num(curr);
        if (num == -1) {
            fprintf(stderr, "Dictionary error: not a lowercase letter\n");
            return;
        }
        // if necessary, write new T9 node
        if (currNode->branches[num - 2] == NULL) {
            T9* node = makeT9(num, NULL);
            currNode->branches[num - 2] = node;
        }
        currNode = currNode->branches[num - 2];
        // if end of the word is reaches, store entire words
        // under the current node
        if (word[i+1] == '\0') {
            makeWord(currNode, word);
        }
    }
}

char* PredictT9(T9* dict, const char* nums) {
    T9* currNode = dict;
    if (currNode == NULL || nums == NULL || strlen(nums) == 0) {
        return NULL;
    }
    int shift = 0;
    int branch;
    for (int i = 0; nums[i] != 0; i++) {
        // handles input, exit this method for invalid inputs
        if (nums[i] == '#') {
            shift += 1;
        } else if (nums[i] >= '2' && nums[i] <= '9') {
            if (shift != 0) {
                // hits a 2-9 number but # is already seem
                return NULL;
            }
            branch = (int) nums[i] - '2';
            currNode = currNode->branches[branch];
            // handles combination not in current T9
            if (currNode == NULL) {
                return NULL;
            }
        } else {
            // handles all input other than "2-9" and "#"
            return NULL;
        }
    }

    // handles shift, a default 0 shift will shift to the word branch
    currNode = currNode->branches[8];

    for (int i = 0; i < shift; i++) {
        if (currNode) {
            currNode = currNode->branches[8];
        } else {
            // handles no target string while shifting
            return NULL;
        }
    }
    shift--;

    // get the word after traversing by numbers
    if (currNode != NULL && currNode->word != NULL) {
        return currNode->word;
    }

    return NULL;
}

void DestroyT9(T9* dict) {
    // base case
    if (dict == NULL) {
        return;
    }
    for (int i = 0; i < 9; i++) {
        if (dict->branches[i] != NULL) {
            DestroyT9(dict->branches[i]);
        }
    }
    // free itself after freeing all branches,
    // when the word pointer is not null, free the word pointer too.
    if (dict->word != NULL) {
        free(dict->word);
    }
    free(dict);
}

/*converts to T9, usage in adding words to T9
Takes in lowercase alphabetical letters
output: 2-9: T9 correspondances
-1: incorrect input
*/ 
int char2Num(char letter) {
    if (letter == 'a' || letter == 'b' || letter == 'c') {
        return 2;
    } else if (letter == 'd' || letter == 'e' || letter == 'f') {
        return 3;
    } else if (letter == 'g' || letter == 'h' || letter == 'i') {
        return 4;
    } else if (letter == 'j' || letter == 'k' || letter == 'l') {
        return 5;
    } else if (letter == 'm' || letter == 'n' || letter == 'o') {
        return 6;
    } else if (letter == 'p' || letter == 'q' || letter == 'r' ||
                letter == 's') {
        return 7;
    } else if (letter == 't' || letter == 'u' || letter == 'v') {
        return 8;
    } else if (letter == 'w' || letter == 'x' || letter == 'y' ||
                letter == 'z') {
        return 9;
    }
    return -1;
}

/* Creates a T9 node with specified number representing characters
int number: THe integer number representing the character in T9
const char* word: the C string, if any, to be added to T9 node
*/
T9* makeT9(int number, const char* word) {
    // everything set to NULL by default to avoid random mem error
    T9* result = (T9*)malloc(sizeof(T9));
    if (result == NULL) {
        fprintf(stderr, "malloc fail at creating T9 node\n");
        return NULL;
    }
    result->number = number;
    if (word) {
        size_t len = strlen(word);
        result->word = malloc(len + 1);
        if (result == NULL) {
            fprintf(stderr, "malloc fail at adding word\n");
            return NULL;
        }
        strncpy(result->word, word, len);
        result->word[len] = '\0';
    } else {
        result->word = NULL;
    }
    for (int i = 0; i < 9; i++) {
        result->branches[i] = NULL;
    }
    return result;
}

/* Add a word to the location of the trie identified as end of a word
node: the pointer to the node that is end of word. A repeated word down
the chain will do nothing
word: contains the word to be added
*/
void makeWord(T9* node, const char* word) {
    T9* currNode = node;
    while (currNode->branches[WORD_LOCATION] != NULL) {
        char* strCopy = currNode->branches[WORD_LOCATION]->word;
        if (strcmp(strCopy, word) == 0 && strlen(word) == strlen(strCopy)) {
            return;
        }
        currNode = currNode->branches[WORD_LOCATION];
    }
    T9* wordNode = makeT9(0, word);
    currNode->branches[WORD_LOCATION] = wordNode;
}

/* Specifically used for file reading
trim line buffer into a word, takes away new line marker if any
dynamically allocate memory for the word returned
line[]: a buffer from fgets, where only one word in on a line
*/
char* trimWord(char line[]) {
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        len--;
    }
    char* word = malloc(len + 1);
    if (word == NULL) {
        fprintf(stderr, "malloc fail at trim word\n");
        return NULL;
    }
    strncpy(word, line, len);
    // Null-terminator
    word[len] = '\0';
    return word;
}

