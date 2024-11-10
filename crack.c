#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Include for toupper
#include "md5.h"
#include "fileutil.h"

#define HASH_LEN 33 // MD5 hashes are 32 characters + null terminator
#define WORD_LEN 100 // Maximum length for dictionary words

// Function to hash the given word and return its MD5 hash
char* TryWord(char *word) 
{
    return md5(word, strlen(word));
}

// Comparator function for qsort
int CompareHashes(const void *a, const void *b) 
{
    return strcmp(*(const char **)a, *(const char **)b);
}

// Function to create variations of the given word and check them against the hashes
void CreateAndCheckVariants(char *word, char **hashes, int hashCount, int *foundCount)
{
    char variant[WORD_LEN];
    char *generatedHash;

    // Original word
    generatedHash = TryWord(word);
    if (LinearSearchAA(generatedHash, hashes, hashCount)) 
    {
        printf("%s %s\n", generatedHash, word);
        (*foundCount)++;
    }
    free(generatedHash);

    // Append digits (0-9)
    for (int i = 0; i < 10; i++) 
    {
        snprintf(variant, WORD_LEN, "%s%d", word, i);
        generatedHash = TryWord(variant);
        if (LinearSearchAA(generatedHash, hashes, hashCount)) 
        {
            printf("%s %s\n", generatedHash, variant);
            (*foundCount)++;
        }
        free(generatedHash);
    }

    // Prepend digits (0-9)
    for (int i = 0; i < 10; i++) 
    {
        snprintf(variant, WORD_LEN, "%d%s", i, word);
        generatedHash = TryWord(variant);
        if (LinearSearchAA(generatedHash, hashes, hashCount)) 
        {
            printf("%s %s\n", generatedHash, variant);
            (*foundCount)++;
        }
        free(generatedHash);
    }

    // Append symbols !, *, _
    const char symbols[] = "!*_";
    for (int i = 0; i < strlen(symbols); i++) 
    {
        snprintf(variant, WORD_LEN, "%s%c", word, symbols[i]);
        generatedHash = TryWord(variant);
        if (LinearSearchAA(generatedHash, hashes, hashCount)) 
        {
            printf("%s %s\n", generatedHash, variant);
            (*foundCount)++;
        }
        free(generatedHash);
    }

    // Capitalize the first character
    strcpy(variant, word);
    variant[0] = toupper(variant[0]);
    generatedHash = TryWord(variant);
    if (LinearSearchAA(generatedHash, hashes, hashCount)) 
    {
        printf("%s %s\n", generatedHash, variant);
        (*foundCount)++;
    }
    free(generatedHash);

    // Convert the entire word to uppercase
    for (int i = 0; i < strlen(word); i++) 
    {
        variant[i] = toupper(word[i]);
    }
    variant[strlen(word)] = '\0';
    generatedHash = TryWord(variant);
    if (LinearSearchAA(generatedHash, hashes, hashCount)) 
    {
        printf("%s %s\n", generatedHash, variant);
        (*foundCount)++;
    }
    free(generatedHash);

    // Replace common characters 'a' -> '@', 's' -> '$'
    strcpy(variant, word);
    for (int i = 0; i < strlen(variant); i++) 
    {
        if (variant[i] == 'a') variant[i] = '@';
        else if (variant[i] == 's') variant[i] = '$';
    }
    generatedHash = TryWord(variant);
    if (LinearSearchAA(generatedHash, hashes, hashCount)) 
    {
        printf("%s %s\n", generatedHash, variant);
        (*foundCount)++;
    }
    free(generatedHash);
}

int main(int argc, char *argv[]) 
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <hash file> <dictionary file>\n", argv[0]);
        return 1;
    }

    // Load hashes from the hash file using LoadFileAA
    int hashCount;
    char **hashes = LoadFileAA(argv[1], &hashCount);
    if (!hashes) 
    {
        perror("Error loading hashes");
        return 1;
    }

    // Sort the hashes using qsort
    qsort(hashes, hashCount, sizeof(char *), CompareHashes);

    // Open the dictionary file for reading
    FILE *dictFile = fopen(argv[2], "r");
    if (!dictFile) 
    {
        perror("Error opening dictionary file");
        FreeAA(hashes, hashCount);
        return 1;
    }

    char word[WORD_LEN];
    int foundCount = 0;

    // Iterate over each word in the dictionary
    while (fgets(word, sizeof(word), dictFile)) 
    {
        // Remove the newline character from the word
        word[strcspn(word, "\n")] = '\0';

        // Check the original word and its variations
        CreateAndCheckVariants(word, hashes, hashCount, &foundCount);
    }

    // Close the dictionary file
    fclose(dictFile);

    // Free the memory allocated for hashes
    FreeAA(hashes, hashCount);

    // Display the number of hashes found
    printf("Number of hashes found: %d\n", foundCount);

    return 0;
}
