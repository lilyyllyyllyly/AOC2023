#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PRINT 1
#define VERBOSE 1

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

// -1 values will be ignored
long min(long a, long b, long c) {
	long min = a;
	if (b != -1 && (min == -1 || b < min)) min = b;
	if (c != -1 && (min == -1 || c < min)) min = c;
	return min;
}

// Transforms the range input_range using map.
// output_small: values that were unchanged because they are SMALLER than the map source range's start (unchanged if none)
// output_inter: transformed values (intersection between the input range and map source range) (NULL if none)
// output_large: values that were unchanged because they are LARGER than the map source range's end (start + length-1) (unchanged if none)
void transform_range(struct range input_range, struct map map, struct range* output_small, struct range* output_inter, struct range* output_large) {
	long input_last = input_range.start + input_range.length-1;
	long map_last = map.source.start + map.source.length-1;

	if (PRINT && VERBOSE) printf("transform:\n\tinput_range: %ld-%ld (%ld)\n", input_range.start, input_last, input_range.length);

	if (input_last < map.source.start) { // If the input range doesn't intersect with the map's source range at all (too SMALL)
		*output_small = input_range;
		if (PRINT && VERBOSE) puts("\ttoo small");
		return;
	}
	
	if (input_range.start > map_last) { // If the input range doesn't intersect with the map's source range at all (too LARGE)
		*output_large = input_range;
		if (PRINT && VERBOSE) puts("\ttoo large");
		return;
	}

	// After this, we know the input range and map source range intersect.

	struct range inter = {0, 0};
	struct range small = {0, 0};
	struct range large = {0, 0};

	if (input_range.start < map.source.start) { // There are number outside the intersection because they are too SMALL
		small.start = input_range.start;
		small.length = map.source.start - small.start;
		
		*output_small = small;
	}

	if (input_last > map_last) { // There are number outside the intersection because they are too LARGE
		large.start = map_last + 1;
		large.length = input_last + 1 - large.start;

		*output_large = large;
	}

	inter.start = input_range.start < map.source.start? map.source.start : input_range.start;
	inter.length = input_range.length - output_small->length - output_large->length;

	inter.start += map.dest.start - map.source.start; // Transforming values inside the intersection

	*output_inter = inter;

	if (PRINT && VERBOSE) printf("\tsmall: %ld-%ld (%ld)\n", small.start, small.start + small.length-1, small.length);
	if (PRINT && VERBOSE) printf("\tinter: %ld-%ld (%ld)\n", inter.start, inter.start + inter.length-1, inter.length);
	if (PRINT && VERBOSE) printf("\tlarge: %ld-%ld (%ld)\n", large.start, large.start + large.length-1, large.length);
}

// Go through all the maps, transforming the seeds range and the resulting ranges from the transform operation recursively.
// I will not attempt to explain this better. I guess you can try to decipher 'diagram.png'. This would be the 'X' function there.
long get_minimum(struct range seeds, struct map** map_arrays, int max_arr, int* max_map, int curr_arr, int curr_map) {
	if (PRINT) printf("get_minimum:\n\tseeds: %ld-%ld (%ld) - map %d-%d\n\n", seeds.start, seeds.start + seeds.length-1, seeds.length, curr_arr, curr_map);
	if (PRINT && VERBOSE) putchar('\t');

	// Transforming seeds
	struct range small, inter, large;
	transform_range(seeds, map_arrays[curr_arr][curr_map], &small, &inter, &large);

	long inter_value = inter.start;
	long small_value = small.start;
	long large_value = large.start;

	// For the seeds which were transformed (in the intersection), go to the next map array
	if (inter.length > 0 && curr_arr + 1 < max_arr)
		inter_value = get_minimum(inter, map_arrays, max_arr, max_map, curr_arr + 1, 0);
	
	// For the ones that weren't, try the next map inside the array to see if they will be transformed
	if (curr_map + 1 < max_map[curr_arr]) {
		if (small.length > 0) small_value = get_minimum(small, map_arrays, max_arr, max_map, curr_arr, curr_map + 1);
		if (large.length > 0) large_value = get_minimum(large, map_arrays, max_arr, max_map, curr_arr, curr_map + 1);
	} else if (curr_arr + 1 < max_arr) {
		// If they weren't transformed after going throught the last map, go to next map array (if exists)
		if (small.length > 0) small_value = get_minimum(small, map_arrays, max_arr, max_map, curr_arr + 1, 0);
		if (large.length > 0) large_value = get_minimum(large, map_arrays, max_arr, max_map, curr_arr + 1, 0);
	}

	// Ignoring empty ranges in min
	if (small.length <= 0) small_value = -1;
	if (inter.length <= 0) inter_value = -1;
	if (large.length <= 0) large_value = -1;

	// Returning min
	long minimum = min(small_value, inter_value, large_value);
	if (PRINT) printf("final (min): %ld\n", minimum);
	return minimum;
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
		if (PRINT && VERBOSE) printf("seed range: %ld-%ld (%ld)\n", seed_ranges[i].start, seed_ranges[i].start + seed_ranges[i].length-1, seed_ranges[i].length);
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
	
	if (PRINT) printf("\n-------------\n\n");

	long minimum;
	for (int i = 0; i < seedrng_count; ++i) {
		struct range seeds = seed_ranges[i];
		long curr_min = get_minimum(seeds, map_arrays, maparr_count, maparr_lengths, 0, 0);
		if (i == 0 || curr_min < minimum) minimum = curr_min;

		if (PRINT) putchar('\n');
	}
	printf("\nmin: %ld\n", minimum);
	
//	for (int j = 0; j < maparr_count; ++j) {
//		for (int k = 0; k < maparr_lengths[j]; ++k) {
//			struct map map = map_arrays[j][k];
//
//			if (PRINT && VERBOSE) printf("transforming seed range:\n\tseed range: %ld-%ld (%ld)\n\tmap: %ld-%ld (%ld)\n\n", seeds.start, seeds.start + seeds.length-1, seeds.length, map.source.start, map.source.start + map.source.length-1, map.source.length);
//			
//			struct range small, inter, large;
//
//			transform_range(seeds, map, &small, &inter, &large);
//
//			if (PRINT && VERBOSE) {
//				puts("\nresults:");
//				printf("\tsmall: %ld-%ld (%ld)\n", small.start, small.start + small.length-1, small.length);
//				printf("\tlarge: %ld-%ld (%ld)\n", large.start, large.start + large.length-1, large.length);
//				printf("\tinter: %ld-%ld (%ld)\n", inter.start, inter.start + inter.length-1, inter.length);
//				putchar('\n');
//			}
//		}
//	}

	for (int i = 0; i < maparr_count; ++i) {
		free(map_arrays[i]);
	}
	free(seed_ranges);
	free(line);
	return 0;
}

