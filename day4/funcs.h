#ifndef FUNCS
#define FUNCS

#include "day4.h"

void getRowsCols(FILE *fp, int *rows, int *maxCols);
void getGrid(FILE *fp, char ***grid, int *rows, int *cols);
int checkAccessible(char **grid, int ***accessible, int rows, int cols);
void newGrid(char **grid, int **accessible, int rows, int cols);

#endif
