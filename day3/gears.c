#include <stdio.h>
#include <stdlib.h>

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
	long fsize = ftell(file);
	fseek(file, 0L, SEEK_SET);

	// column count
	size_t alloc = sizeof(char);
	char* line = malloc(alloc);
	int cols = getline(&line, &alloc, file); // -1 to remove \n
	free(line);
	fseek(file, 0L, SEEK_SET);

	// row count
	int rows = (int)fsize/(cols); // \n needs to be counted here
	
	// allocating scheme buffer;
	char* scheme = calloc(fsize, sizeof(char));
	fread(scheme, sizeof(char), fsize, file);
	fclose(file);
	// ---

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols-1; j++) {
			printf("%c", scheme[(i*cols)+j]);
		}
		printf("\n");
	}

	free(scheme);
	return 0;
}

