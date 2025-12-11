#include "funcs.h"
#include "day7.h"

void reverse(char s[]){
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

void itoa(long int n, char s[]){
	int i;
	long int sign;

	if((sign = n) < 0)
		n = -n;
	i = 0;
	do{
		s[i++] = (n % 10) + '0';
	} while((n /= 10) > 0);
	
	if(sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

int iina(int i, int a[], size_t size){
	for(int j = 0; j < (int)size; j++){
		if(a[j] == i) return 1;
	}
	return 0;
}

long int lineToI(int *l, size_t w){
	long int I = 0;
	for(int i = 0; i < (int)w; i++)
		I = (I*10)+l[i];
	return I;	
}

void getRawGrid(FILE *fp, char ***grid, size_t *width, size_t *height){
	rewind(fp);
	int nL = 0, nC = 0;
	char c;
	while(fscanf(fp, "%c", &c) != EOF){
		if(nL == 0) nC++;
		nL += (c == '\n');
	}
	nC--;	// Don't count \n
	*width = nC;
	*height = nL;

	(*grid) = (char**) malloc(sizeof(char*) * nL);
	for(int i = 0; i < nL; i++)
		(*grid)[i] = (char*) malloc(sizeof(char) * nC);

	rewind(fp);
	ssize_t nread;
	size_t buffBytes;
	char *line = NULL;
	int i = 0;
	while((nread = getline(&line, &buffBytes, fp)) != -1){
		line[nread-1] = '\0';
		(*grid)[i++] = line;
		line = NULL;
	}
	rewind(fp);
}

void getSplitters(char **grid, int ***splitters, size_t width, size_t height, size_t *nb){
	*nb = 0;

	for(int i = 0; i < (int)height; i++){
		for(int j = 0; j < (int)width; j++){
			if(grid[i][j] == '^'){
				(*splitters)[i][j] = 1;
				(*nb)++;
			}
			else
				(*splitters)[i][j] = 0;
		}
	}
}

void getBeams(char **grid, int ***beams, size_t width){
	for(int i = 0; i < (int)width; i++){
		if(grid[0][i] == 'S'){
			(*beams)[0][i] = 2;
			(*beams)[1][i] = 2;
		}
	}
}

void makeVisualization(char **grid, int **intGrid, size_t width, size_t height, int asChars){
	if(grid != NULL){ 
		printf("Grid dimensions: %ldx%ld\n", width, height);

		for(int i = 0; i < (int)height; i++){
			for(int j = 0; j < (int)width; j++)
				printf("%c", grid[i][j]);
			printf("\n");
		}
		printf("\n");
	}
	else if(grid == NULL && intGrid != NULL && asChars == 1){
		for(int i = 0; i < (int)height; i++){
			for(int j = 0; j < (int)width; j++){
				switch(intGrid[i][j]){
					case 0:
						printf(".");
						break;
					case 1:
						printf("^");
						break;
					case 2:
						printf("|");
						break;
					case 3:
						printf("^");
						break;
					case 4:
						printf("x");
						break;
					default:
						break;
				}
			}
			printf("\n");
		}
		printf("\n");
	}
	else if(intGrid != NULL){
		printf("Grid dimensions: %ldx%ld\n", width, height);

		for(int i = 0; i < (int)height; i++){
			for(int j = 0; j < (int)width; j++)
				printf("%2d ", intGrid[i][j]);
			printf("\n");
		}
		printf("\n");
	}
}

inline void showGrid(char **grid, int **intGrid, size_t width, size_t height){
	makeVisualization(grid, intGrid, width, height, 0);
}
inline void showGridChars(char **grid, int **intGrid, size_t width, size_t height){
	makeVisualization(grid, intGrid, width, height, 1);
}

int makeGrids(FILE *fp, char ***grid, int ***splitMask, int ***beamMask, int ***finalMask, size_t *width, size_t *height,\
		size_t *nbSplitters){
	getRawGrid(fp, grid, width, height);
	
	// Create masks
	*splitMask = (int**) malloc(sizeof(int*) * (*height));
	*beamMask = (int**) malloc(sizeof(int*) * (*height));
	*finalMask = (int**) malloc(sizeof(int*) * (*height));
	if(*splitMask == NULL || *beamMask == NULL || *finalMask == NULL)
		return 1;

	*nbSplitters = 0;
	for(int i = 0; i < (int)*height; i++){
		(*splitMask)[i] = (int*) malloc(sizeof(int) * (*width));
		(*beamMask)[i] = (int*) malloc(sizeof(int) * (*width));
		(*finalMask)[i] = (int*) malloc(sizeof(int) * (*width));

		if((*splitMask)[i] == NULL || (*beamMask)[i] == NULL || (*finalMask)[i] == NULL)
			return 1;
	}
	getSplitters(*grid, splitMask, *width, *height, nbSplitters);
	getBeams(*grid, beamMask, *width);

	// Make total mask
	for(int i = 0; i < (int)*height; i++)
		for(int j = 0; j < (int)*width; j++)
			(*finalMask)[i][j] = (*splitMask)[i][j] + (*beamMask)[i][j];

	return 0;
}

int countSplits(int ***finalMask, size_t width, size_t height){
	int splits = 0;

	// Store the occupied positions for fast access
	int occupiedX[width], newlyOccupiedX[width];
	for(int i = 0; i < (int)width; i++)
		occupiedX[i] = -1;
	int nOcc = 0, newOcc = 0;
	for(int w = 0; w < (int)width; w++)
		if((*finalMask)[0][w])
			occupiedX[nOcc++] = w;

	// Count the number of splits
	int w;
	for(int h = 2; h < (int)height-1; h += 2){
		// Reset newly occupied list
		for(int i = 0; i < (int)width; i++)
			newlyOccupiedX[i] = -1;
		newOcc = 0;

		// Check for beams in front of the splitters
		for(int n = 0; n < nOcc; n++){
			w = occupiedX[n];

			// Splitter at (h, w)
			if((*finalMask)[h-1][w] + (*finalMask)[h][w] == 3){
				(*finalMask)[h][w] = 3;
				splits++;
				if(w-1 >= 0){
					if((*finalMask)[h][w-1] != 2){
						(*finalMask)[h][w-1] = 2;
						(*finalMask)[h+1][w-1] = 2;
						if(!iina(w-1, newlyOccupiedX, newOcc))
							newlyOccupiedX[newOcc++] = w-1;
					}
				}
				if(w+1 < (int)width){
					if((*finalMask)[h][w+1] != 2){
						(*finalMask)[h][w+1] = 2;
						(*finalMask)[h+1][w+1] = 2;
						if(!iina(w+1, newlyOccupiedX, newOcc))
							newlyOccupiedX[newOcc++] = w+1;
					}
				}
			}
			// No splitter in the way
			else if((*finalMask)[h-1][w] + (*finalMask)[h][w] == 2){
				if((*finalMask)[h][w] != 2){
					(*finalMask)[h][w] = 2;
					(*finalMask)[h+1][w] = 2;
					if(!iina(w, newlyOccupiedX, newOcc))
						newlyOccupiedX[newOcc++] = w;
				}
			}
		}
		// Copy newly occupied positions to currently occupied positions
		for(int i = 0; i < (int)width; i++){
			occupiedX[i] = newlyOccupiedX[i];
		}
		nOcc = newOcc;
	}
	return splits;
}

long int numberOfPaths(Node *firstNode, Node ****nodes, size_t width, size_t height){
	long int n = 0, nNext = 0;
	int nextX, nextY;
	Node *nextNode = NULL;

	for(int i = 0; i < 2; i++){
		for(int h = firstNode->y + 2; h < (int)height; h += 2){
			nextY = h;
			nextX = firstNode->x - 1 + (i*2);		// i=0 -> x-1 ; i=1 -> x+1

			nextNode = (*nodes)[nextY][nextX];
			if(nextNode != NULL){
				firstNode->nbNextNodes += (firstNode->nbNextNodes == -1) + 1;

				if(nextNode->paths != -1)
					n += nextNode->paths;
				else{
					nextNode->x = nextX;
					nextNode->y = nextY;
					nNext = numberOfPaths(nextNode, nodes, width, height);
					if(nNext == 0) n++;
					else n += nNext;
				}
				break;			// Stop descending when a splitter has been reached
			}
		}
		if(nextNode == NULL) n += 1;
	}
	firstNode->paths = n;
	return n;
}
