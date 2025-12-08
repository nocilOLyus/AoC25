#ifndef FUNCS
#define FUNCS

#include "day6.h"

int getNumberLines(FILE *fp);
int getNumberCols(char *line, ssize_t length);
int getNbDigits(long n);
int getWorksheetProps(FILE *fp, int *nbProblems, Problem **problems);
long int solveProblems(Problem *problems, int nbProblems);
void getCephalopodMath(Problem **problems, int nbProblems);

#endif
