#include "day2.h"

int main(int argc, char *argv[]){
	if(argc == 1){
		printf("Please specify an input file\n");
		return 1;
	}

	char *filename = argv[1];

	// Get the number of ranges
	FILE *fp = fopen(filename, "r");
	char c;
	int nb_ranges = 0;
	while(fscanf(fp, "%c", &c) != EOF)
		nb_ranges += (c == '-');
	printf("Number of ranges: %d\n", nb_ranges);

	// Get the ranges
	fp = freopen(filename, "r", fp);
	Range *ranges = (Range *) malloc(nb_ranges * sizeof(Range));

	for(int i = 0; i < nb_ranges; i++)
		fscanf(fp, "%ld-%ld,", &(ranges[i].start), &(ranges[i].end));

	int nb_digits;
	long part1 = 0, part2 = 0;
	for(int i = 0; i < nb_ranges; i++){
		for(long j = ranges[i].start; j <= ranges[i].end; j++){
			// Check if the number has an even number of digits
			if(((nb_digits = get_nb_digits(j)) % 2) == 0)
				// Check if number is a valid id (part 1)
				if(!is_valid_id_p1(j, nb_digits))
					part1 += j;	

			// Check if number is a valid id (part 2)
			if(!is_valid_id_p2(j, nb_digits))
				part2 += j;
		}
	}
	
	printf("Part1: %ld\n", part1);
	printf("Part2: %ld\n", part2);

	return 0;
}

int get_nb_digits(long n){
	int nb = 1;
	while((n = n / 10) != 0)
		nb++;

	return nb;
}

static int is_valid_id_p1(long n, int nb_digits){
	int fh, sh;
	int window = nb_digits / 2;
	fh = n / (pow(10, window));	
	sh = n - (fh * pow(10, window));
	
	return (fh != sh);
}

static int is_valid_id_p2(long n, int nb_digits){
	int win_max = nb_digits / 2;
	int nb_win = 0;
	int w = 0;
	long nw;
	for(int i = 1; i <= win_max; i++){
		w = n / pow(10, nb_digits - i);
		nb_win = nb_digits / i;
		nw = 0;
		for(int j = 0; j < nb_win; j++)
			nw += w * pow(10, i*j);
		if(n == nw)
			return 0;
	}
	return 1;
}
