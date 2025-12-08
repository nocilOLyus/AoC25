#include "funcs.h"
#include "day6.h"

void reverse(char s[])
 {
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

int getNumberLines(FILE *fp){
	int n = 0;
	char c;
	while(fscanf(fp, "%c", &c) != EOF)
		n += (c == '\n');

	return n;
}

int getNumberCols(char *line, ssize_t length){
	int nbCols = 0;
	int prevSpace = 0;

	for(int i = 0; i < length; i++){
		if(line[i] == ' ' && prevSpace == 0){
			prevSpace = 1;
			nbCols++;
		}
		else if(line[i] != ' '){
			prevSpace = 0;
		}
	}

	return nbCols+1;
}

int getNbDigits(long n){
	int nb = 1;
	while((n = n / 10) != 0)
		nb++;

	return nb;
}

int getWorksheetProps(FILE *fp, int *nbProblems, Problem **problems){
	char *line = NULL, *lastLine = NULL;
	size_t buffBytes;
	ssize_t nread, last_nread;
	int nbOperands;

	nbOperands = getNumberLines(fp) - 1;
	rewind(fp);

	// Read the lines and store the last line read to get the very last line of the file
	while((nread = getline(&line, &buffBytes, fp)) != -1){
		lastLine = line;
		last_nread = nread;
	}
	
	// Count the number of operators/problems
	int nbSpaces = 0;
	for(int i = 0; i < last_nread-1; i++)		// -1 removes "\n" at the end of the line
		nbSpaces += (lastLine[i] == ' ');
	*nbProblems = last_nread - nbSpaces - 1;

	char *operators = (char*) malloc(sizeof(char) * (*nbProblems));
	int oi = 0;
	for(int i = 0; i < last_nread; i++){
		if(lastLine[i] == ' ')
			continue;
		operators[oi++] = lastLine[i];
	}

	// Allocate memory for each problem
	*problems = (Problem*) malloc(sizeof(Problem) * (*nbProblems));
	for(int i = 0; i < *nbProblems; i++){
		(*problems)[i].operands = (int*) malloc(sizeof(int) * (nbOperands));
		(*problems)[i].operator = operators[i];
		
		// Set default alignment value
		(*problems)[i].align = UNKNOWN;
		(*problems)[i].nbOperands = nbOperands;
	}

	// Populate the operands of each problem
	rewind(fp);
	for(int i = 0; i < nbOperands; i++)
		for(int j = 0; j < *nbProblems; j++)
			if(fscanf(fp, "%d ", &((*problems)[j].operands[i])) == 0)
				return 1;

	// Determine the maximum number of digits in every problem
	int maximax = 0, maxNbDigits, nbDigits;
	for(int p = 0; p < *nbProblems; p++){
		maxNbDigits = 0;
		for(int i = 0; i < nbOperands; i++)
			if((nbDigits = getNbDigits((*problems)[p].operands[i])) > maxNbDigits)
				maxNbDigits = nbDigits;
		(*problems)[p].maxNbDigits = maxNbDigits;
		if(maxNbDigits > maximax)
			maximax = maxNbDigits;
	}

	// Determine the alignment of every problem
	rewind(fp);
	char c;
	char *numStr = (char*) malloc(sizeof(char));
	//char numStr[maximax];
	for(int i = 0; i < nbOperands; i++){
		for(int j = 0; j < *nbProblems; j++){
			maxNbDigits = (*problems)[j].maxNbDigits;
			
			if((*problems)[j].align != UNKNOWN){
				for(int k = 0; k <= maxNbDigits; k++)
					fgetc(fp);
				continue;
			}

			numStr = (char*) realloc((void*)numStr, sizeof(char) * maxNbDigits);
			for(int k = 0; k < maxNbDigits; k++){
				if((c = fgetc(fp)) != EOF)
					numStr[k] = c;
				else
					return 1;
			}

			if(numStr[0] == ' ')
				(*problems)[j].align = RIGHT;
			else if(numStr[maxNbDigits-1] == ' ')
				(*problems)[j].align = LEFT;

			fgetc(fp);

			if(i == nbOperands-1 && (*problems)[j].align == UNKNOWN)
				(*problems)[j].align = RIGHT;
		}
	}
	return 0;
}

long int solveProblems(Problem *problems, int nbProblems){
	long double sol = 0;
	long double p;
	for(int i = 0; i < nbProblems; i++){
		p = 0;
		switch(problems[i].operator){
			case '+':
				for(int j = 0; j < problems[i].nbOperands; j++)
					p += problems[i].operands[j];
				break;
			case '*':
				p = problems[i].operands[0];
				for(int j = 1; j < problems[i].nbOperands; j++)
					p *= problems[i].operands[j];
				break;
			default:
				break;
		}
		sol += p;
	}
	return sol;
}

void getCephalopodMath(Problem **problems, int nbProblems){
	int maxNbDigits, nbDigits, d;
	int pos;
	char c, *numStr;
	numStr = (char*) malloc(sizeof(char));

	for(int p = 0; p < nbProblems; p++){
		maxNbDigits = (*problems)[p].maxNbDigits;

		long int newOperands[maxNbDigits];
		for(int n = maxNbDigits-1; n >= 0; n--){
			newOperands[n] = 0;

			// Aligned right: easy division
			if((*problems)[p].align == RIGHT){
				for(int i = 0; i < (*problems)[p].nbOperands; i++){
					d = (*problems)[p].operands[i] / pow(10, n);
					if(d != 0){
						newOperands[n] = (newOperands[n] * 10) + d;
						(*problems)[p].operands[i] -= d * pow(10, n);
					}
				}
			}

			// Aligned left: convert to string and add padding, then convert back to number while ignoring padding
			else if((*problems)[p].align == LEFT){
				numStr = (char*) realloc((void*)numStr, sizeof(char)*maxNbDigits);
				pos = (maxNbDigits-1)-n;
				for(int i = 0; i < (*problems)[p].nbOperands; i++){
					itoa((*problems)[p].operands[i], numStr);

					nbDigits = getNbDigits((*problems)[p].operands[i]);
					if(nbDigits < maxNbDigits)
						for(int k = nbDigits; k < maxNbDigits; k++)
							numStr[k] = '.';

					if((c = numStr[pos]) != '.'){
						d = c - '0';
						newOperands[n] = (newOperands[n] * 10) + d;
					}
				}
			}
		}

		// Change the operands in problem
		(*problems)[p].operands = (int*) realloc((void*)(*problems)[p].operands, sizeof(int) * maxNbDigits);
		for(int i = 0; i < maxNbDigits; i++)
			(*problems)[p].operands[i] = newOperands[i];

		(*problems)[p].nbOperands = maxNbDigits;
	}
}
