#include "day3.h"
#include "funcs.h"

int main(int argc, char *argv[]){
	if(argc == 1){
		printf("Please specify an input file");
		return 1;
	}

	char *filename = argv[1];

	// Get the number of ranges
	FILE *fp = fopen(filename, "r");
	if(fp == NULL){
		printf("Error opening file\n");
		return 2;
	}
	
	// Get the number of banks
	int nb_banks = getNumberLines(fp);
	printf("Number of lines: %d\n", nb_banks);

	fp = freopen(filename, "r", fp);

	// Store banks
	Bank banks[nb_banks];
	if(getBanks(fp, banks, nb_banks) != 0){
		printf("Error reading banks\n");
		return 3;
	}

	fclose(fp);

	// Part 1: total joltage1
	int part1 = 0;
	long int part2 = 0;
	for(int i = 0; i < nb_banks; i++){
		part1 += banks[i].joltage1;
		part2 += banks[i].joltage2;
	}

	printf("Part 1: %d\n", part1);
	printf("Part 2: %ld\n", part2);


	for(int i = 0; i < nb_banks; i++){
		free(banks[i].batteries);
	}
	return 0;
}

