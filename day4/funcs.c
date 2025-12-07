#include "funcs.h"
#include "day4.h"

void getRowsCols(FILE *fp, int *rows, int *maxCols){
	int nb_lines = 0, maxLineLength = 0;
	char *line = NULL;
	size_t bufferBytes;
	ssize_t nread;

	while((nread = getline(&line, &bufferBytes, fp)) != -1){
		nb_lines++;
		if((int)nread - 1 > maxLineLength)
			maxLineLength = (int)nread - 1;
	}

	*rows = nb_lines;
	*maxCols = maxLineLength;
}

void getGrid(FILE *fp, char ***grid, int *rows, int *cols){
	getRowsCols(fp, rows, cols);
	*grid = (char**) malloc(sizeof(char*) * (*rows));
	for(int i = 0; i < *rows; i++)
		(*grid)[i] = (char*) malloc(sizeof(char) * (*cols));

	rewind(fp);

	ssize_t nread;
	size_t buffBytes;
	char *line = (char*) malloc(sizeof(char) * (*cols));
	int nline = 0;
	while((nread = getline(&line, &buffBytes, fp)) != -1){
		line[nread-1] = '\0';
		strcpy((*grid)[nline], line);		
		nline++;

		line = NULL;
	}
	free(line);
}	

int checkAccessible(char **grid, int ***accessible, int rows, int cols){
	int n = 0, rolls = 0;
	char **paddedGrid = (char**) malloc(sizeof(char*) * (rows+2));
	*accessible = (int**) malloc(sizeof(int*) * rows);

	for(int i = 0; i < rows+2; i++){
		paddedGrid[i] = (char*) malloc(sizeof(char*) * (cols+2));
		(*accessible)[i] = (int*) calloc(cols, sizeof(int));

		if(i == 0 || i == rows+1){
			for(int j = 0; j < cols+2; j++)
				paddedGrid[i][j] = '.';
		}
		else{
			paddedGrid[i][0] = '.';
			paddedGrid[i][cols+1] = '.';
			for(int j = 1; j < rows+1; j++)
				paddedGrid[i][j] = grid[i-1][j-1];
		}
	}

	for(int i = 1; i < rows+1; i++){
		for(int j = 1; j < cols+1; j++){
			if(paddedGrid[i][j] == '@'){
				rolls = -1;		// Remove the center roll
				for(int k = i-1; k <= i+1; k++){
					for(int h = j-1; h <= j+1; h++){
						rolls += (paddedGrid[k][h] == '@');
					}
				}
				n += (rolls < 4);
				(*accessible)[i-1][j-1] = (rolls < 4);
			}
		}
	}

	return n;
}

void newGrid(char **grid, int **accessible, int rows, int cols){
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < cols; j++)
			if(accessible[i][j])
				grid[i][j] = '.';
}
