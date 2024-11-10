#ifndef FILEUTIL_H
#define FILEUTIL_H

#define COLS 100 // For the 2D array width

// Load the dictionary of words from the given filename.
// Return a pointer to the array of strings.
// Sets the value of size to be the number of valid entries in the array (not the total array length).
char **LoadFileAA(char *filename, int *size);
char (*LoadFile2D(char *filename, int *size))[COLS];

// Search the dictionary for the target string
// Return the found string or NULL if not found.
char *SubstringSearchAA(char *target, char **arr, int size);
char *SubstringSearch2D(char *target, char (*arr)[COLS], int size);
char *LinearSearchAA(char *target, char **arr, int size);

// Free the memory used by the array
void FreeAA(char **arr, int size);
void Free2D(char (*arr)[COLS]);

#endif // FILEUTIL_H