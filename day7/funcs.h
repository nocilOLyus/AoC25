#ifndef FUNCS
#define FUNCS

#include "day7.h"

void reverse(char s[]);
void itoa(long int n, char s[]);
int iina(int i, int a[], size_t size);
long int lineToI(int *l, size_t w);
void getRawGrid(FILE *fp, char ***grid, size_t *width, size_t *height);
void getSplitters(char **grid, int ***splitters, size_t width, size_t height, size_t *nb);
void getBeams(char **grid, int ***beams, size_t width);
void makeVisualization(char **grid, int **intGrid, size_t width, size_t height, int asChars);
void showGrid(char **grid, int **intGrid, size_t width, size_t height);
void showGridChars(char **grid, int **intGrid, size_t width, size_t height);
int makeGrids(FILE *fp, char ***grid, int ***splitMask, int ***beamMask, int ***finalMask, size_t *width, size_t *height,\
		size_t *nbSplitters);
int countSplits(int ***finalMask, size_t width, size_t height);
long int numberOfPaths(Node *firstNode, Node ****nodes, size_t width, size_t height);

#endif
