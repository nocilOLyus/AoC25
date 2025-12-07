#ifndef FUNCS
#define FUNCS

#include "day5.h"

int getNumberRanges(FILE *fp);
int getRanges(FILE *fp, Range **ranges);
int resolveOverlaps(Range **ranges, Range **newRanges, int nbRanges);

#endif
