#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileutil.h"

// Load the text file into an array of arrays (array of strings)
char **LoadFileAA(char *filename, int *size)
{
    FILE *in = fopen(filename, "r");
    if (!in)
    {
        perror("Can't open file");
        exit(1);
    }
    
    int capacity = 10;
    char **arr = malloc(capacity * sizeof(char *));
    *size = 0;
    char buffer[1000];

    while (fgets(buffer, sizeof(buffer), in))
    {
        // Remove newline character if present
        buffer[strcspn(buffer, "\n")] = '\0';

        // Expand array if necessary
        if (*size == capacity)
        {
            capacity += 10;
            arr = realloc(arr, capacity * sizeof(char *));
        }

        // Allocate memory for string and copy buffer
        arr[*size] = malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(arr[*size], buffer);
        (*size)++;
    }

    fclose(in);
    return arr;
}

// Load the text file into a 2D array
char (*LoadFile2D(char *filename, int *size))[COLS]
{
    FILE *in = fopen(filename, "r");
    if (!in)
    {
        perror("Can't open file");
        exit(1);
    }

    int capacity = 10;
    char (*arr)[COLS] = malloc(capacity * sizeof(char[COLS]));
    *size = 0;
    char buffer[1000];

    while (fgets(buffer, sizeof(buffer), in))
    {
        buffer[strcspn(buffer, "\n")] = '\0';

        if (*size == capacity)
        {
            capacity += 10;
            arr = realloc(arr, capacity * sizeof(char[COLS]));
        }

        strcpy(arr[*size], buffer);
        (*size)++;
    }

    fclose(in);
    return arr;
}

// Search array-of-arrays for target string
char *SubstringSearchAA(char *target, char **lines, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (strstr(lines[i], target))
        {
            return lines[i];
        }
    }
    return NULL;
}

// Search 2D array for target string
char *SubstringSearch2D(char *target, char (*lines)[COLS], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (strstr(lines[i], target))
        {
            return lines[i];
        }
    }
    return NULL;
}

// Linear search for an exact match in array-of-arrays
char *LinearSearchAA(char *target, char **lines, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(lines[i], target) == 0)
        {
            return lines[i];
        }
    }
    return NULL;
}

// Free memory used by array-of-arrays
void FreeAA(char **arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

// Free memory used by 2D array
void Free2D(char (*arr)[COLS])
{
    free(arr);
}