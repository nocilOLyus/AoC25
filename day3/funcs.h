#include "day3.h"

int getNumberLines(FILE *);
void rotate(long int arr[], size_t n, int dir, int start);
int getBanks(FILE *fp, Bank banks[], size_t nb_banks);
long int getJoltage(int *batteries, size_t nb_bats, int nb_digits);
int *convertToI(char *bank, size_t nb_bats);
void sort(int *batteries, size_t nb_bats);
int compar(const void *a, const void *b);
