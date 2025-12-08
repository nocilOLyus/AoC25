#include "day6.h"
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

	// Get the number of operands and problems
	int nbProblems;
	Problem *problems;
	if(getWorksheetProps(fp, &nbProblems, &problems) == 1){
		printf("Error fetching data\n");
		return 1;
	}

	long int part1 = solveProblems(problems, nbProblems);

	// Read the problems as cephalopod math
	getCephalopodMath(&problems, nbProblems);
	long int part2 = solveProblems(problems, nbProblems);

	printf("Part 1: %ld\n", part1);
	printf("Part 2: %ld\n", part2);

	free(problems);

	return 0;
}
