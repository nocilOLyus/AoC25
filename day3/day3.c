#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

typedef struct{
	size_t nb_bats;
	int joltage1;
	long int joltage2;
	int *batteries;
} Bank;

int getNumberLines(FILE *);
void rotate(long int arr[], size_t n, int dir, int start);
int getBanks(FILE *fp, Bank banks[], size_t nb_banks);
long int getJoltage(int *batteries, size_t nb_bats, int nb_digits);
int *convertToI(char *bank, size_t nb_bats);
void sort(int *batteries, size_t nb_bats);
int compar(const void *a, const void *b);

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

int getNumberLines(FILE *fp){
	int n = 0;
	char c;
	while(fscanf(fp, "%c", &c) != EOF)
		n += (c == '\n');

	return n;
}

int getBanks(FILE *fp, Bank banks[], size_t nb_banks){
	ssize_t nread;
	size_t n = 0;
	char *line;
	for(int i = 0; i < (int)nb_banks; i++){
		line = NULL;
		nread = getline(&line, &n, fp);
		banks[i].nb_bats = nread-1;
		if(nread == -1){
			printf("Problem reading line %d\n", i+1);
			return 3;
		}
		line[nread-1] = '\0';

		banks[i].batteries = convertToI(line, banks[i].nb_bats);
		banks[i].joltage1 = getJoltage(banks[i].batteries, banks[i].nb_bats, 2);
		banks[i].joltage2 = getJoltage(banks[i].batteries, banks[i].nb_bats, 12);
	}
	free(line);
	return 0;
}

long int getJoltage(int *batteries, size_t nb_bats, int nb_digits){
	int digit;
	long int joltage = 0;
	long int max[nb_digits];
	for(int i = 0; i < nb_digits; i++)
		max[i] = 0;

	for(int i = 0; i < (int)nb_bats; i++){
		digit = batteries[i];
		// Optimize battery choice
		for(int j = 0; j < nb_digits-1; j++){
			if(max[j] < max[j+1]){
				rotate(max, nb_digits, -1, j);
				max[nb_digits-1] = digit;
				break;
			}
		}
		if(max[nb_digits-1] < digit){
			max[nb_digits-1] = digit;
		}
	}
	for(int i = 0; i < nb_digits; i++)
		joltage = (joltage * 10) + max[i];
	return joltage;
}

void rotate(long int arr[], size_t n, int dir, int start){
	long int rot[n];
	int offset;

	if(start != 0)
		for(int i = 0; i < start; i++)
			rot[i] = arr[i];

	for(int i = start; i < (int)n; i++){
		offset = (i-dir) % n;
		while(offset < 0)
			offset = ((n-1)+offset) % n;	
		rot[i] = arr[offset];
	}

	for(int i = start; i < (int)n; i++){
		arr[i] = rot[i];
	}
}

int *convertToI(char *bank, size_t nb_bats){
	int *bankI = (int*) malloc(nb_bats * sizeof(int));
	for(int i = 0; i < (int)nb_bats; i++){
		bankI[i] = (int)bank[i] - 48;
	}
	return bankI;
}
