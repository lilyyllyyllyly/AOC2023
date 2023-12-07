#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

char* scheme;
int rows, cols;
long fsize;

void print_scheme() {
	for (int i = 0; i < fsize; ++i) putchar(scheme[i]);
}

int getnum(char* digit) {
	// Moving to first digit of number
	int pos = (intptr_t)digit - (intptr_t)scheme;
	for (pos = (intptr_t)digit - (intptr_t)scheme;
	     pos-1 >= 0 && scheme[pos-1] <= '9' && scheme[pos-1] >= '0';
	     --pos) {
	}
	char* start = &scheme[pos];

	// Getting value of number and end point
	char* end = NULL;
	int value = strtol(start, &end, 10);
	int endpoint = end? (intptr_t)end - (intptr_t)start : fsize;

	printf("end: %d\n", endpoint);

	// Replacing with . to ignore next time
	for (int i = 0; i < endpoint; ++i) {
		start[i] = '.';
	}

	printf("number value: %d\n\n", value);
	print_scheme();

	return value;
}

// returns the gear ratio if symbol is a gear, otherwise 0
int check_adjacent(char* symbol) {
	int ccols = cols-1; // cols without the \n
	int pos = (intptr_t)symbol - (intptr_t)scheme;
	int symbolx = pos%cols, symboly = pos/cols;

	// Looking for part numbers adjacent to the symbol
	int* parts = calloc(2, sizeof(int));
	int curr_part = 0;
	printf("pos: %d | x: %d | y: %d\n", pos, symbolx, symboly);

	for (int currx = symbolx-1; currx <= symbolx+1; ++currx) {
	for (int curry = symboly-1; curry <= symboly+1; ++curry) {
		if (currx == symbolx && curry == symboly) continue; // the asterisk itself, not a neighbour
		if (currx < 0 || currx >= ccols || curry < 0 || curry >= rows) continue; // out of bounds
		
		char curr = scheme[(curry*cols)+currx];
		if (curr > '9' | curr < '0') continue; // isnt number
		printf("found a number adjacent to symbol: %c\n", curr);

		if (curr_part >= 2) {
			free(parts);
			printf("not a gear: too many part numbers\n");
			return 0;
		}

		parts[curr_part] = getnum(&scheme[(curry*cols)+currx]);
		++curr_part;
	}
	}

	if (curr_part < 2) {
		free(parts);
		printf("not a gear: too few part numbers\n");
		return 0;
	}

	int mult = parts[0] * parts[1];
	free(parts);
	return mult;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "ERROR: schematic file not specified!\n");
		return 1;
	}

	FILE* file = fopen(argv[1], "r");
	if (!file) {
		fprintf(stderr, "ERROR: could not open file %s.\n", argv[1]);
		return 1;
	}

	// file size
	fseek(file, 0L, SEEK_END);
	fsize = ftell(file);
	fseek(file, 0L, SEEK_SET);

	// column count
	size_t alloc = sizeof(char);
	char* line = malloc(alloc);
	cols = getline(&line, &alloc, file);
	free(line);
	fseek(file, 0L, SEEK_SET);

	// row count
	rows = (int)fsize/(cols);
	//
	
	printf("cols: %d | rows: %d\n", cols, rows);
	
	// allocating scheme buffer;
	scheme = calloc(fsize, sizeof(char));
	fread(scheme, sizeof(char), fsize, file);
	fclose(file);
	// ---
	
	print_scheme();

	long sum = 0;
	for (int i = 0; i < fsize; ++i) {
		char* curr = &scheme[i];
		if (*curr != '*') continue;

		puts("found *\n");
		int ratio;
		if ((ratio = check_adjacent(curr))) {
			printf("It is a gear!\nratio: %d\n\n", ratio);
			sum += ratio;
		}
	}
	free(scheme);

	printf("\n---\nsum: %ld\n", sum);

	return 0;
}

