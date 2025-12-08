#ifndef DAY6H
#define DAY6H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

enum Alignment{
	RIGHT,
	LEFT,
	UNKNOWN
};

typedef struct{
	int *operands;
	int nbOperands;
	char operator;
	int maxNbDigits;
	enum Alignment align;
} Problem;

#endif
