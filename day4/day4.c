#include "day4.h"
#include "funcs.h"

int visualize = 0;

int main(int argc, char **argv){
	if(argc == 1){
		printf("Please provide an input file\n");
		return 1;
	}

	if(argc == 3 && strcmp(argv[2], "-v") == 0){
		visualize = 1;	
	}

	char *filename = argv[1];
	FILE *fp = fopen(filename, "r");
	if(fp ==  NULL){
		printf("Error opening file\n");
		return 1;
	}

	int rows, cols;
	char **grid;
	getGrid(fp, &grid, &rows, &cols);

	// Print grid geometry and content
	if(visualize){
		printf("Initial grid\n");
		printf("Rows: %d\nCols: %d\n", rows, cols);
		for(int i = 0; i < rows; i++){
			for(int j = 0; j < cols; j++)
				printf("%c", grid[i][j]);
			printf("\n");
		}
	}

	int part1;
	int **accessible;
	part1 = checkAccessible(grid, &accessible, rows, cols);

	int part2 = 0, removed = part1;
	while(removed != 0){
		part2 += removed;
		newGrid(grid, accessible, rows, cols);
		
		// Print grid geometry and content
		if(visualize){
			printf("Remove %d rolls\n", removed);
			for(int i = 0; i < rows; i++){
				for(int j = 0; j < cols; j++)
					printf("%c", grid[i][j]);
				printf("\n");
			}
			printf("\n");
		}

		removed = checkAccessible(grid, &accessible, rows, cols);
	}

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	for(int i = 0; i < rows; i++){
		free(grid[i]);
		free(accessible[i]);
	}
	return 0;
}
