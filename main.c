#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_WORD_SIZE 30
#define STR_HELPER(x) #x
#define STRINGIFY(x) STR_HELPER(x)

char* strdup(const char* str) {
    size_t len = strlen(str) + 1; // +1 for the null terminator
    char* new_str = malloc(len);

    if (new_str == NULL) {
        return NULL; // Memory allocation failed
    }

    strcpy(new_str, str);
    return new_str;
}

typedef struct {
    char** words;
    int size;
} WordArray;

WordArray extractWordsFromFile(const char *filePath) {
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_WORD_SIZE + 1]; // +1 for null terminator
    int capacity = 10; // Initial capacity of the array, you can adjust as needed
    int size = 0; // Number of words read
    char** words = malloc(capacity * sizeof(char*));

    while (fscanf(file, "%" STRINGIFY(MAX_WORD_SIZE) "s", buffer) == 1) {
        if (size == capacity) {
            // If the array is full, resize it
            capacity *= 2;
            words = realloc(words, capacity * sizeof(char*));
        }

       // Allocate memory for the word and copy it using custom strdup
        words[size] = strdup(buffer);

        size++;
    }

    fclose(file);

    // Trim the array to the actual size
    words = realloc(words, size * sizeof(char*));

    WordArray result = { .words = words, .size = size };
    return result;
}

void freeWordArray(WordArray* array) {
    for (int i = 0; i < array->size; i++) {
        free(array->words[i]);
    }
    free(array->words);
}

int isConsonant(char c) {
    return strchr("bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ", c) != NULL;
}

bool hasDoubleConsonants(const char* word) {
    int length = strlen(word);
    
    for (int i = 0; i < length - 1; i++) {
        if (isConsonant(word[i]) && word[i] == word[i + 1]) {
            return true; 
        }
    }

    return false;
}

WordArray getWordsWithDoubleConsonants(const WordArray* array) {
    WordArray result = { .words = NULL, .size = 0 };

    for (int i = 0; i < array->size; i++) {
        if (hasDoubleConsonants(array->words[i])) {
            // If the word has double consonants, add it to the result array
            result.words = realloc(result.words, (result.size + 1) * sizeof(char*));
            result.words[result.size] = strdup(array->words[i]);
            result.size++;
        }
    }

    return result;
}

bool isDuplicate(const char* word, const WordArray* array) {
    for (int i = 0; i < array->size; i++) {
        if (strcmp(word, array->words[i]) == 0) {
            return true;
        }
    }
    return false;
}

WordArray getUniqueWords(const WordArray* array) {
    WordArray result = { .words = NULL, .size = 0 };

    for (int i = 0; i < array->size; i++) {
        if (!isDuplicate(array->words[i], &result)) {
            // If the word is not a duplicate, add it to the result array
            result.words = realloc(result.words, (result.size + 1) * sizeof(char*));
            result.words[result.size] = strdup(array->words[i]);
            result.size++;
        }
    }

    return result;
}

void printWordArray(const WordArray* array) {
    for (int i = 0; i < array->size; i++) {
        printf("%s\n", array->words[i]);
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filePath = argv[1];

    WordArray words = extractWordsFromFile(filePath);
    WordArray processedWords = getWordsWithDoubleConsonants(&words);
    WordArray result = getUniqueWords(&processedWords);

    printWordArray(&result);

    freeWordArray(&words);
    freeWordArray(&processedWords);
    freeWordArray(&result);

    return EXIT_SUCCESS;
}
