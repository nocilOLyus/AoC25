#include "day7.h"
#include "funcs.h"

int visualize = 0;

int main(int argc, char *argv[]){
	if(argc == 1){
		printf("Please provide an input file\n");
		return 1;
	}

	if(argc == 3 && strcmp(argv[2], "-v") == 0){
		visualize = 1;	
	}

	char *filename = argv[1];
	FILE *fp = fopen(filename, "r");
	if(fp == NULL){
		printf("Error opening file\n");
		return 1;
	}

	// Make grids
	char **grid;
	int **splitMask, **beamMask, **finalMask;
	size_t width, height, nbSplitters;
	if(makeGrids(fp, &grid, &splitMask, &beamMask, &finalMask, &width, &height, &nbSplitters) == 1){
		printf("Failed allocating memory");
		return 1;
	}

	// Visualize grid
	//if(visualize) showGrid(grid, (int**)NULL, width, height);
	//if(visualize) showGrid(NULL, finalMask, width, height);

	int nbSplits = countSplits(&finalMask, width, height);

	// Visualize final grid
	if(visualize) showGridChars(NULL, finalMask, width, height);

	printf("Number of splits: %d\n", nbSplits);

	// Count the number of universes = paths through the graph
	// Make array storing node pointers at splitter positions
	Node ***nodes = (Node***) calloc(height, sizeof(Node**));
	Node *firstNode = (Node*) calloc(1, sizeof(Node));;
	for(int h = 0; h < (int)height; h++){
		nodes[h] = (Node**) calloc(width, sizeof(Node*));
		for(int w = 0; w < (int)width; w++){
			if(finalMask[h][w] == 3){
				if(h == 2) nodes[h][w] = firstNode;
				else nodes[h][w] = (Node*) calloc(1, sizeof(Node));

				nodes[h][w]->x = w;
				nodes[h][w]->y = h;
				nodes[h][w]->nbNextNodes = -1;		// Unitialized values
				nodes[h][w]->paths = -1;
			}
		}
	}

	long int nbPaths = numberOfPaths(firstNode, &nodes, width, height);
	printf("Number of paths: %ld\n", nbPaths);

	// Close program
	fclose(fp);

	for(int i = 0; i < (int)height; i++){
		free(splitMask[i]);
		free(beamMask[i]);
		free(finalMask[i]);
		free(nodes[i]);
	}
	free(splitMask);
	free(beamMask);
	free(finalMask);

	return 0;
}
