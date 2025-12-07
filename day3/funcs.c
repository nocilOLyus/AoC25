#include "day3.h"
#include "funcs.h"

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
