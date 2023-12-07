#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

char* scheme;
int rows, cols;
long fsize;

int check_adjacent(char* digit) {
	int pos = (intptr_t)digit - (intptr_t)scheme;
	int digitx = pos

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
	cols = getline(&line, &alloc, file); // -1 to remove \n
	free(line);
	fseek(file, 0L, SEEK_SET);

	// row count
	rows = (int)fsize/(cols); // \n needs to be counted here
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
		if (check_adjacent(curr)) printf("It is a part number!\n");
	}
	free(scheme);

	printf("sum: %ld", sum);

	return 0;
}

