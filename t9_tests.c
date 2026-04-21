#include <string.h>

#include "safe_assert.h"
#include "t9_lib.h"
#include <stdio.h>

void AssertReturnedStringEquals(char* expected, char* actual);

suite("T9") {
  test("Empty initialization") {
    T9* dict = InitializeEmptyT9();
    safe_assert(dict != NULL);
    DestroyT9(dict);
  }
  

  test("ab, ba, ca") {
    T9* dict = InitializeFromFileT9("simple.txt");
    safe_assert(dict != NULL);
    char* word = PredictT9(dict, "22");
    AssertReturnedStringEquals("ab", word);
    word = PredictT9(dict, "22#");
    AssertReturnedStringEquals("ba", word);
    word = PredictT9(dict, "22##");
    AssertReturnedStringEquals("ca", word);
    
    DestroyT9(dict);
  }
  

  test("EmptyDictAndAdd") {
    T9* dict = InitializeEmptyT9();
    safe_assert(dict != NULL);
    
    AddWordToT9(dict, "book");
    AddWordToT9(dict, "cool");

    char* word = PredictT9(dict, "2665#");
    AssertReturnedStringEquals("cool", word);

    DestroyT9(dict);
  }

  test("FristAndLastWord") {
    T9* dict = InitializeFromFileT9("small_dictionary.txt");
    safe_assert(dict != NULL);
    char* word = PredictT9(dict, "22738275");
    AssertReturnedStringEquals("aardvark", word);
    word = PredictT9(dict, "93272");
    AssertReturnedStringEquals("zebra", word);
    
    DestroyT9(dict);
  }

  
  test("MultipleDictInstantiation") {
    T9* dict = InitializeEmptyT9();
    T9* dict1 = InitializeEmptyT9();
    T9* dict2 = InitializeFromFileT9("small_dictionary.txt");
    T9* dict3 = InitializeFromFileT9("small_dictionary.txt");
    safe_assert(dict != NULL);
    safe_assert(dict1 != NULL);
    safe_assert(dict2 != NULL);
    safe_assert(dict3 != NULL);
    DestroyT9(dict);
    DestroyT9(dict1);
    DestroyT9(dict2);
    DestroyT9(dict3);
  }

  test ("NonfileReturnsNullNError") {
    T9* dict = InitializeFromFileT9("notAFile.txt");
    safe_assert(dict == NULL);
  }

  test("SearchInSmallDict") {
    T9* dict = InitializeFromFileT9("small_dictionary.txt");
    safe_assert(dict != NULL);

    char* word = PredictT9(dict, "#");
    safe_assert(word == NULL);
    word = PredictT9(dict, "639");
    AssertReturnedStringEquals("new", word);
    word = PredictT9(dict, "639###");
    safe_assert(word == NULL);
    DestroyT9(dict);
  }

  test("SearchMultiplesSmallDict") {
    T9* dict = InitializeFromFileT9("small_dictionary.txt");
    safe_assert(dict != NULL);

    char* word = PredictT9(dict, "76257");
    AssertReturnedStringEquals("rocks", word);
    word = PredictT9(dict, "76257#");
    AssertReturnedStringEquals("socks", word);
    word = PredictT9(dict, "76257##");
    safe_assert(word == NULL);

    DestroyT9(dict);
  }

  test("MultipleSearch") {
    T9* dict = InitializeFromFileT9("small_dictionary.txt");

    char *word = PredictT9(dict, "99");
    safe_assert(word == NULL);
    word = PredictT9(dict, "87");
    AssertReturnedStringEquals("up", word);
    word = PredictT9(dict, "2662");
    AssertReturnedStringEquals("anna", word);
    
    DestroyT9(dict);
  }

  test("LongerSearchWithDict") {
    T9* dict = InitializeFromFileT9("dictionary.txt");
    safe_assert(dict != NULL);

    char* word = PredictT9(dict, "#");
    safe_assert(word == NULL);
    word = PredictT9(dict, "639");
    AssertReturnedStringEquals("mew", word);
    word = PredictT9(dict, "639#");
    AssertReturnedStringEquals("new", word);
    DestroyT9(dict);
  }

  test("AddWordOnDict") {
    T9* dict = InitializeFromFileT9("small_dictionary.txt");
    safe_assert(dict != NULL);

    AddWordToT9(dict, "aaaab");
    AddWordToT9(dict, "aaaaa");
    AddWordToT9(dict, "aaaac");

    char* word = PredictT9(dict, "22222");
    AssertReturnedStringEquals("aaaab", word);
    word = PredictT9(dict, "22222#");
    AssertReturnedStringEquals("aaaaa", word);
    word = PredictT9(dict, "22222##");
    AssertReturnedStringEquals("aaaac", word);
    word = PredictT9(dict, "22222###");
    safe_assert(word == NULL);

    DestroyT9(dict);
  }

  test("AddForbiddenStrHasNoEffect") {
    T9* dict = InitializeEmptyT9();

    AddWordToT9(dict, "APE");
    AddWordToT9(dict, "OHHHH");
    AddWordToT9(dict, "\n");
    AddWordToT9(dict, "\0");

    char* word = PredictT9(dict, "273");
    safe_assert(word == NULL);
    word = PredictT9(dict, "64444");
    safe_assert(word == NULL);

    DestroyT9(dict);
  }

  test("AddVeryLongString") {
    T9* dict = InitializeEmptyT9();
    int length = 10;
    char* veryLong = malloc(length);
    char* longInput = malloc(length);
    veryLong[length] = "\0";
    longInput[length] = "\0";
    for (int i = 0; i < length-1; i++) {
      veryLong[i] = 'a';
      longInput[i] = '2';
    }
    AddWordToT9(dict, veryLong);
    char* word = PredictT9(dict, longInput);
    AssertReturnedStringEquals(veryLong, word);
    free(veryLong);
    veryLong = NULL;
    free(longInput);
    longInput = NULL;
    DestroyT9(dict);
  }

  test("longInputToPrediction") {
    T9* dict = InitializeFromFileT9("small_dictionary.txt");
    int length = 900;
    char* longInput = malloc(length);
    longInput[length] = "\0";
    for (int i = 0; i < length-1; i++) {
      longInput[i] = '2' + (i % 8);
    }
    char* word = PredictT9(dict, longInput);
    safe_assert(word == NULL);
    free(longInput);
    longInput = NULL;
    DestroyT9(dict);
  }

  test("AddAlreadyDefinedWordDoesNothing") {
    T9* dict = InitializeFromFileT9("dictionary.txt");

    char* word = PredictT9(dict, "48759#");
    AssertReturnedStringEquals("husky", word);
    AddWordToT9(dict, "husky");
    char* word1 = PredictT9(dict, "48759##");
    safe_assert(word1 == NULL);
    DestroyT9(dict);
  }

  test("SpecialCharacters") {
    T9* dict = InitializeFromFileT9("dictionary.txt");

    char* word = PredictT9(dict, "##48759");
    safe_assert(word == NULL);
    word = PredictT9(dict, "\0");
    safe_assert(word == NULL);
    word = PredictT9(dict, "\n");
    safe_assert(word == NULL);
    word = PredictT9(dict, NULL);
    safe_assert(word == NULL);
    word = PredictT9(dict, " ");
    safe_assert(word == NULL);

    DestroyT9(dict);
  }

  test("FoulInputs") {
    T9* dict = InitializeFromFileT9("small_dictionary.txt");
    safe_assert(dict != NULL);
    char* word = PredictT9(dict, "unparsable input");
    safe_assert(word == NULL);
    word = PredictT9(dict, "#2#2#4");
    safe_assert(word == NULL);
    word = PredictT9(dict, "111111");
    safe_assert(word == NULL);
    word = PredictT9(dict, "        ");
    
    DestroyT9(dict);
  }

  test("PredictDoesNotMatchLength") {
    T9* dict = InitializeEmptyT9();

    AddWordToT9(dict, "rob");
    AddWordToT9(dict, "robust");
    AddWordToT9(dict, "robu");

    char* word = PredictT9(dict, "762878");
    safe_assert(strlen(word) == 6)
    word = PredictT9(dict, "762");
    safe_assert(strlen(word) == 3)
    word = PredictT9(dict, "7628");
    safe_assert(strlen(word) == 4)

    DestroyT9(dict);
  }

  test ("addingRepeated") {
  T9* dict = InitializeEmptyT9();
    safe_assert(dict != NULL);
    
    AddWordToT9(dict, "abab");
    AddWordToT9(dict, "abab");

    char* word = PredictT9(dict, "2222");
    AssertReturnedStringEquals("abab", word);
    word = PredictT9(dict, "2222#");
    safe_assert(word == NULL);
    DestroyT9(dict);
  }

  test("SingleLine file") {
    T9* dict = InitializeFromFileT9("singleLine.txt");
    safe_assert(dict != NULL);
    char* word = PredictT9(dict, "5463");
    AssertReturnedStringEquals("line", word);
    
    DestroyT9(dict);
  }

  test("addWord pre condition") {
    T9* dict = InitializeEmptyT9();
    
    
    AddWordToT9(dict, NULL);
    safe_assert(dict != NULL);
    AddWordToT9(dict, "293847");

    char* word = PredictT9(dict, NULL);
    safe_assert(word == NULL);

    DestroyT9(dict);
  }

  test("bad dictionary test") {
    T9* dict = InitializeFromFileT9("bad.txt");
    safe_assert(dict != NULL);
    char* word = PredictT9(dict, "46");
    AssertReturnedStringEquals("go", word);
    word = PredictT9(dict, "46#");
    safe_assert(word == NULL);
    word = PredictT9(dict, "223");
    AssertReturnedStringEquals("ace", word);
    word = PredictT9(dict, "2722342");
    AssertReturnedStringEquals("arcadia", word);
    word = PredictT9(dict, "7325");
    safe_assert(word == NULL);
    word = PredictT9(dict, "99");
    safe_assert(word == NULL);
    
    DestroyT9(dict);
  }

  test("Letter Mapping") {
    T9* dict = InitializeEmptyT9();
    T9* dict1 = InitializeEmptyT9();
    AddWordToT9(dict, "abcdefghijklmnopqrstuvwxyz");
    char* lowerCase = PredictT9(dict, "22233344455566677778889999");
    AssertReturnedStringEquals("abcdefghijklmnopqrstuvwxyz", lowerCase);
    AddWordToT9(dict1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    char* upper = PredictT9(dict1, "22233344455566677778889999");
    safe_assert(upper == NULL);
    DestroyT9(dict);
    DestroyT9(dict1);
    // fix: in makeT9, I did sizeof(word) which is actually the size
    // of a pointer
  }
}

void AssertReturnedStringEquals(char* expected, char* actual) {
  safe_assert(actual > 0);
  safe_assert(strlen(actual) == strlen(expected));
  safe_assert(strcmp(actual, expected) == 0);
}
