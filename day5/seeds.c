#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

	size_t alloc = sizeof(char);
	char* line = malloc(alloc);
	int len;
	char* curr;

	// Counting seeds
	len = getline(&line, &alloc, file);
	curr = line;

	while ((intptr_t)curr - (intptr_t)line < len && (*curr > '9' || *curr < '0')) curr = &curr[1]; // Move to first digit
	char* first_digit = curr;
	printf("f - %c\n", *first_digit);

	int seed_count = 0;
	while (curr) {
		char* end = NULL;
		strtol(curr, &end, 10);

		if (curr == end) break;
		curr = end;

		++seed_count;
	}
	printf("seed count:%d\n", seed_count);

	// Getting seeds
	curr = first_digit;
	long* values = malloc(seed_count * sizeof(long));
	for (int i = 0; i < seed_count; ++i) {
		values[i] = strtol(curr, &curr, 10);
		printf("seed: %ld\n", values[i]);
	}
	//
	
	for (int i = 0; i < seed_count; ++i) {
		fseek(file, 0L, SEEK_SET);
		len = getline(&line, &alloc, file);

		while (len != -1) {
			// Going to next range title
			while ((len = getline(&line, &alloc, file)) != -1 && (line[0] < 'a' || line[0] > 'z')) ;

			// Mapping to range
			while ((len = getline(&line, &alloc, file)) != -1 && (line[0] <= '9' && line[0] >= '0')) {
				curr = line;

				long dest_start, source_start, range_size;
				dest_start   = strtol(curr, &curr, 10);
				source_start = strtol(curr, &curr, 10);
				range_size   = strtol(curr, &curr, 10);

				if (values[i] >= source_start && values[i] < source_start + range_size) {
					values[i] += dest_start - source_start;
					printf("new: %ld\n", values[i]);
					break;
				}
				printf("didnt fit range.\n");
			}
		}

		printf("final: %ld\n\n", values[i]);
	}

	long min = values[0];
	printf("final value for seed 0: %ld\n", values[0]);
	for (int i = 1; i < seed_count; ++i) {
		printf("final value for seed %d: %ld\n", i, values[i]);
		if (values[i] < min) min = values[i];
	}

	printf("\nmin: %ld\n", min);

	free(values);
	free(line);
	fclose(file);
	return 0;
}

