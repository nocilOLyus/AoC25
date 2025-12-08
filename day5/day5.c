#include "day5.h"
#include "funcs.h"

int main(int argc, char *argv[]){
	if(argc == 1){
		printf("Please provide an input file\n");
		return 1;
	}

	char *filename = argv[1];
	FILE *fp = fopen(filename, "r");
	if(fp == NULL){
		printf("Error opening file\n");
		return 1;
	}

	Range *rawRanges;
	int totalRanges = getRanges(fp, &rawRanges);
	Range *ranges;
	int overlaps = resolveOverlaps(&rawRanges, &ranges, totalRanges);
	int nbRanges = totalRanges - overlaps;
	free(rawRanges);

	//printf("Number of overlaps: %d\n", overlaps);
	// Print simplified ranges
	//printf("New ranges:\n");
	//for(int i = 0; i < nbRanges; i++)
	//	printf("%ld-%ld\n", ranges[i].start, ranges[i].end);

	// Part 1: get the number of fresh ingredient IDs
	long int id;
	int fresh = 0;
	while(fscanf(fp, "%ld\n", &id) != EOF)
		for(int i = 0; i < nbRanges; i++)
			fresh += (id >= ranges[i].start && id <= ranges[i].end);
	fclose(fp);
	printf("Number of fresh ingredient IDs: %d\n", fresh);

	// Part 2: get the number of IDs the ranges consider to be fresh
	long int part2 = 0;
	for(int i = 0; i < nbRanges; i++)
		part2 += ranges[i].end - ranges[i].start + 1;
	printf("Number of IDs that are considered fresh: %ld\n", part2);

	free(ranges);
	return 0;
}
