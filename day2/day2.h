#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int get_nb_digits(long n);
static int is_valid_id_p1(long n, int nb_digits);
static int is_valid_id_p2(long n, int nb_digits);

typedef struct{
	long start;
	long end;
} Range;
