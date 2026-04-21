#ifndef T9_PRIV_H_
#define T9_PRIV_H_

typedef struct T9 {
    int number;
    // 9 possible branches
    // The 9th branches stores nodes that contains word
    struct T9* branches[9];
    // if the T9 so far contains a whole word, the word is stored here
    char* word;
} T9;

#endif  // T9_PRIV_H_
