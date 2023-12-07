#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

char* scheme;
int rows, cols;
long fsize;

// returns 1 if there is a symbol adjacent to the character digit, otherwise 0
int check_adjacent(char* start, char* end) {
	int ccols = cols-1; // cols without the \n

	// Looking for symbols adjacent to every digit of the number
	for (int i = 0; i < (intptr_t)end - (intptr_t)start; ++i) {
		char* digit = &start[i];
		printf("checking digit %c...\n", *digit);

		int pos = (intptr_t)digit - (intptr_t)scheme;
		int digitx = pos%cols, digity = pos/cols;
		printf("pos: %d | x: %d | y: %d\n", pos, digitx, digity);

		for (int currx = digitx-1; currx <= digitx+1; ++currx) {
		for (int curry = digity-1; curry <= digity+1; ++curry) {
			if (currx == digitx && curry == digity) continue; // the digit itself, not a neighbour
			if (currx < 0 || currx >= ccols || curry < 0 || curry >= rows) continue; // out of bounds
			
			char curr = scheme[(curry*cols)+currx];
			if (curr == '.' || (curr <= '9' && curr >= '0')) continue; // isnt a symbol

			printf("found a symbol adjacent to digit: %c\n", curr);
			return 1;
		}
		}
	}

	return 0;
}

void print_scheme() {
	for (int i = 0; i < fsize; ++i) putchar(scheme[i]);
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "ERROR: calibration file not specified!\n");
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
		if (*curr > '9' || *curr < '0') continue;

		printf("\nfound digit: %c\n", *curr);
		char* end = NULL;
		int value = strtol(curr, &end, 10);
		if (check_adjacent(curr, end)) {
			printf("It is a part number!\n\n");

			sum += value;
			for (int j = 0; j < (intptr_t)end - (intptr_t)curr; ++j)
				curr[j] = '.'; // replacing with . so its not counted again
			print_scheme();
		}
	}
	free(scheme);

	printf("\n---\nsum: %ld\n", sum);

	return 0;
}

