#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PRINT 1

FILE* file;

int get_location(long seed) {
	if (PRINT) printf("\nseed: %ld\n", seed);

	size_t alloc = sizeof(char);
	char* line = malloc(alloc);
	char* curr = line;
	int len = 0;

	// Go back to start of file
	fseek(file, 0L, SEEK_SET);
	len = getline(&line, &alloc, file);

	// Go through each range to transform the seed value
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

			if (seed >= source_start && seed < source_start + range_size) {
				seed += dest_start - source_start;
				if (PRINT) printf("new: %ld\n", seed);
				break;
			}
		}
	}
	
	// Return to start after moving
	fseek(file, 0L, SEEK_SET);
	len = getline(&line, &alloc, file);

	free(line);
	return seed;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		if (PRINT) fprintf(stderr, "ERROR: calibration file not specified!\n");
		return 1;
	}

	file = fopen(argv[1], "r");
	if (!file) {
		if (PRINT) fprintf(stderr, "ERROR: could not open file %s.\n", argv[1]);
		return 1;
	}

	size_t alloc = sizeof(char);
	char* line = malloc(alloc);
	int len;
	char* curr;

	// Counting seed ranges
	len = getline(&line, &alloc, file);
	curr = line;

	while ((intptr_t)curr - (intptr_t)line < len && (*curr > '9' || *curr < '0')) curr = &curr[1]; // Move to first digit
	char* first_digit = curr;
	if (PRINT) printf("f - %c\n", *first_digit);

	int range_count = 0;
	while (curr) {
		char* end = NULL;
		strtol(curr, &end, 10);

		if (curr == end) break;
		curr = end;

		++range_count;
	}
	range_count /= 2;
	if (PRINT) printf("range count:%d\n", range_count);

	// Allocating and fllling range arrays
	long* range_starts  = malloc(range_count * sizeof(long));
	long* range_lengths = malloc(range_count * sizeof(long));

	curr = first_digit;
	for (int i = 0; i < range_count; ++i) {
		range_starts[i]  = strtol(curr, &curr, 10);
		range_lengths[i] = strtol(curr, &curr, 10);
		if (PRINT) printf("range: %ld - %ld (%ld)\n", range_starts[i], range_starts[i] + range_lengths[i], range_lengths[i]);
	}

	// Getting location for all seeds and find minimum
	long min;

	for (int i = 0; i < range_count; ++i) {
		for (long j = range_starts[i]; j < range_starts[i] + range_lengths[i]; ++j) {
			long value = get_location(j);

			if (i == 0 && j == range_starts[i]) min = value;
			if (value < min) min = value;

			if (PRINT) printf("final: %ld\n", value);
		}
	}
	fclose(file);

	printf("\nmin: %ld\n", min);

	free(range_starts);
	free(range_lengths);
	free(line);
	return 0;
}

