#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PRINT 1

FILE* file;

struct range {
	long dest_start;
	long source_start;
	long length;
};

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

	int seedrng_count = 0;
	while (curr) {
		char* end = NULL;
		strtol(curr, &end, 10);

		if (curr == end) break;
		curr = end;

		++seedrng_count;
	}
	seedrng_count /= 2;
	if (PRINT) printf("seedrng count:%d\n", seedrng_count);

	// Allocating and fllling seed range arrays
	long* seedrng_starts  = malloc(seedrng_count * sizeof(long));
	long* seedrng_lengths = malloc(seedrng_count * sizeof(long));

	curr = first_digit;
	for (int i = 0; i < seedrng_count; ++i) {
		seedrng_starts[i]  = strtol(curr, &curr, 10);
		seedrng_lengths[i] = strtol(curr, &curr, 10);
		if (PRINT) printf("seedrng: %ld - %ld (%ld)\n", seedrng_starts[i], seedrng_starts[i] + seedrng_lengths[i], seedrng_lengths[i]);
	}

	// Filling transform range arrays
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

	// Getting location for all seeds and find minimum
	long min;

	for (int i = 0; i < seedrng_count; ++i) {
		for (long j = seedrng_starts[i]; j < seedrng_starts[i] + seedrng_lengths[i]; ++j) {
			long value = get_location(j);

			if (i == 0 && j == seedrng_starts[i]) min = value;
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

