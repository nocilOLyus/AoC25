#include "funcs.h"
#include "day5.h"

#define MAX(a, b) (a > b) ? a : b
#define MIN(a, b) (a < b) ? a : b

int getNumberRanges(FILE *fp){
	int n = 0;
	ssize_t nread;
	char *line = NULL;
	size_t buffBytes;
	while((nread = getline(&line, &buffBytes, fp)) != -1 && line[0] != '\n')
		n++;
	return n;
}

int getRanges(FILE *fp, Range **ranges){
	int nbRanges = getNumberRanges(fp);
	*ranges = (Range*) malloc(sizeof(Range) * nbRanges);
	rewind(fp);
	for(int i = 0; i < nbRanges; i++)
		fscanf(fp, "%ld-%ld", &((*ranges)[i].start), &((*ranges)[i].end));

	return nbRanges;
}

int resolveOverlaps(Range **ranges, Range **newRanges, int nbRanges){
	long int rS, rE, cS, cE;
	int nbOverlaps = 0, overlap = 0;
	for(int i = 0; i < nbRanges; i++){
		rS = (*ranges)[i].start;
		rE = (*ranges)[i].end;
		if(rS == 0 && rE == 0)
			continue;

		overlap = 0;
		for(int j = i+1; j < nbRanges; j++){
			cS = (*ranges)[j].start;	
			cE = (*ranges)[j].end;	

			if(cS >= rS && cS <= rE){
				rE = MAX(rE, cE);
				overlap = 1;
			}
			else if(cE >= rS && cE <= rE){
				rS = MIN(rS, cS);
				overlap = 1;
			}
			else if(cS < rS && cE > rE){
				rS = cS;
				rE = cE;
				overlap = 1;
			}

			if(overlap){
				// Change the bounds of the current range
				(*ranges)[i].start = rS;
				(*ranges)[i].end = rE;

				// Delete selected range
				(*ranges)[j].start = 0;
				(*ranges)[j].end = 0;

				overlap = 0;
				nbOverlaps++;
				j = i;
			}
		}
	}

	// Make new Range list
	*newRanges = (Range*) malloc(sizeof(Range) * (nbRanges - nbOverlaps));
	int ni = 0;
	for(int i = 0; i < nbRanges; i++){
		rS = (*ranges)[i].start;
		rE = (*ranges)[i].end;
		if(rS == 0 && rE == 0)
			continue;

		(*newRanges)[ni].start = rS;
		(*newRanges)[ni++].end = rE;
	}

	return nbOverlaps;
}
