#include <stdio.h>
#include <stdlib.h>

#define START 50

void getLR(FILE *f, int LR[]);
void getPass(FILE *f, int pass);

int main(int argc, char *argv[]){
	if(argc == 1){
		printf("Please specify an input file\n");
		return 1;
	}

	char *filename = argv[1];

	FILE *fp = fopen(filename, "r");	

	char c, prevC = 0;
	int d, counter = START, part1 = 0, part2 = 0;
	while(fscanf(fp, "%c%d\n", &c, &d) != EOF){
		counter += (c == 'L') ? -d : d;
		part2 += (counter < 0 && counter != -d) + (abs(counter) / 100) + (counter == 0);
		counter = (counter < 0) ? (100 + (counter % 100)) % 100 : counter % 100;
		part1 += (counter ==  0);
	}
	fclose(fp);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	return 0;
} 

void getLR(FILE *fp, int LR[]){
	char c = 0;
	int d = 0;
	while(fscanf(fp, "%c%d\n", &c, &d) != EOF)
		LR[(c == 'L') ? 0 : 1] += d;
}
