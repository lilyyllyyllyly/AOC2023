#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PRINT 0
#define VERBOSE 0

FILE* file;

struct range {
	long start;
	long length;
};

// This "map" is not the same as a "map" from input. It's one line. A "map" from the input would be an array of struct map.
struct map {
	struct range source;
	struct range dest;
};

int transform(long value, struct map map, int* transformed) {
	if (PRINT && VERBOSE) puts("transform:");
	if (PRINT && VERBOSE) printf("\tvalue: %ld\n\tsource: %ld\n\tdest: %ld\n\tlength: %ld\n", value, map.source.start, map.dest.start, map.source.length);

	if (transformed) *transformed = 0;
	if (value >= map.source.start && value < map.source.start + map.source.length) {
		if (VERBOSE) putchar('\t');
		if (PRINT) printf("%ld -> ", value);
		value += map.dest.start - map.source.start;
		if (PRINT) printf("%ld\n", value);

		if (transformed) *transformed = 1;
	}

	return value;
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

	int seedrng_count = 0; // rng is range not random number generator lol
	while (curr) {
		char* end = NULL;
		strtol(curr, &end, 10);

		if (curr == end) break;
		curr = end;

		++seedrng_count;
	}
	seedrng_count /= 2;
	if (PRINT) printf("seed range count:%d\n", seedrng_count);

	// Allocating and fllling seed range arrays
	struct range* seed_ranges  = malloc(seedrng_count * sizeof(*seed_ranges));

	curr = first_digit;
	for (int i = 0; i < seedrng_count; ++i) {
		seed_ranges[i].start  = strtol(curr, &curr, 10);
		seed_ranges[i].length = strtol(curr, &curr, 10);
		if (PRINT && VERBOSE) printf("seed range: %ld - %ld (%ld)\n", seed_ranges[i].start, seed_ranges[i].start + seed_ranges[i].length, seed_ranges[i].length);
	}

	// Creating map arrays array
	  // - Counting map titles
	int maparr_count;
	for (maparr_count = -1; len != -1; ++maparr_count) {
		while ((len = getline(&line, &alloc, file)) != -1 && (line[0] < 'a' || line[0] > 'z')) ; // Going to next map title
	}

	  // - Returning to the start
	fseek(file, 0L, SEEK_SET);
	len = getline(&line, &alloc, file); // Skipping seeds line
	
	  // - Creating array
	struct map* map_arrays[maparr_count];

	if (PRINT) printf("map array count: %d\n", maparr_count);
	// ---
	
	// Allocating memory for arrays inside map arrays
	int maparr_lengths[maparr_count];
	for (int i = 0; len != -1; ++i) {
		while ((len = getline(&line, &alloc, file)) != -1 && (line[0] < 'a' || line[0] > 'z')) ; // Going to next map title
		for (maparr_lengths[i] = 0; (len = getline(&line, &alloc, file)) != -1 && (line[0] <= '9' && line[0] >= '0'); ++maparr_lengths[i]) ; // Counting
		
		map_arrays[i] = malloc(maparr_lengths[i] * sizeof(*map_arrays[i]));

		if (PRINT) printf("map count for array %d: %d\n", i, maparr_lengths[i]);
	}

	  // - Returning to the start
	fseek(file, 0L, SEEK_SET);
	getline(&line, &alloc, file); // Skipping seeds line
	// ---

	// Filling map arrays array
	while ((len = getline(&line, &alloc, file)) != -1 && (line[0] < 'a' || line[0] > 'z')) ; // Going to first map title

	for (int i = 0; i < maparr_count; ++i) {
		for (int j = 0; j < maparr_lengths[i] && (len = getline(&line, &alloc, file)) != -1 && (line[0] <= '9' && line[0] >= '0'); ++j) {
			curr = line;

			map_arrays[i][j].dest.start = strtol(curr, &curr, 10);
			map_arrays[i][j].source.start = strtol(curr, &curr, 10);
			map_arrays[i][j].source.length = map_arrays[i][j].dest.length = strtol(curr, &curr, 10);

			if (PRINT && VERBOSE) printf("map %d-%d:\n\tsource: %ld\n\tdest: %ld\n\tlength: %ld\n", i, j, map_arrays[i][j].source.start, map_arrays[i][j].dest.start, map_arrays[i][j].source.length);
		}

		while ((len = getline(&line, &alloc, file)) != -1 && (line[0] < 'a' || line[0] > 'z')) ; // Going to next map title
	}
	fclose(file);
	
	printf("\n-------------\n\n");
	
	// Calculating minimum location from seed values
	long min = 0;
	for (int i = 0; i < seedrng_count; ++i) {
		for (int j = seed_ranges[i].start; j < seed_ranges[i].start + seed_ranges[i].length; ++j) {
			long seed = j;
			if (PRINT) printf("seed: %ld\n", seed);

			for (int k = 0; k < maparr_count; ++k) {
				int transformed = 0;
				for (int l = 0; l < maparr_lengths[k] && !transformed; l++) {
					if (PRINT) printf("doing map %d-%d\n", k, l);
					seed = transform(seed, map_arrays[k][l], &transformed);
				}
				if (PRINT) putchar('\n');

				if (i == 0 || seed < min) min = seed;
				if (PRINT) printf("final: %ld\n\n", seed);
			}
		}
	}

	printf("\nmin: %ld\n", min);
	//

	for (int i = 0; i < maparr_count; ++i) {
		free(map_arrays[i]);
	}
	free(seed_ranges);
	free(line);
	return 0;
}

