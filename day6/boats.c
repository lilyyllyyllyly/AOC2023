#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// Returns the 2 possible needed hold times (by parameter, min_out and max_out) to REACH the record (not pass it) given the max race time and record time
void record_hold_time(int max, int record, float* min_out, float* max_out) {
	// see math.txt
	*min_out = (-max + sqrt(pow(max, 2) - 4*record))/-2;
	*max_out = (-max - sqrt(pow(max, 2) - 4*record))/-2;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "ERROR: race times file not specified!\n");
		return 1;
	}

	FILE* file = fopen(argv[1], "r");
	if (!file) {
		fprintf(stderr, "ERROR: could not open file %s.\n", argv[1]);
		return 1;
	}
	
	size_t alloc = sizeof(char);
	char* line = malloc(alloc);
	char* curr = NULL;
	int len;

	// Reading file
	  // - Counting races
	len = getline(&line, &alloc, file);
	curr = line;
	while ((*curr > '9' || *curr < '0') && (intptr_t)line - (intptr_t)curr < len) curr = &curr[1]; // Going to first digit
	
	int race_count = 0;
	for (race_count = 0; curr != NULL; ++race_count) strtol(curr, &curr, 10);

	  // - Create arrays for race times and records
	int race_times[race_count];
	int race_records[race_count];

	  // - Filling arrays
	    // - Race time
	curr = line;
	while ((*curr > '9' || *curr < '0') && (intptr_t)line - (intptr_t)curr < len) curr = &curr[1]; // Going to first digit
	for (int i = 0; i < race_count; ++i) race_times[i] = atoi(curr);

	    // - Record time
	len = getline(&line, &alloc, file);
	curr = line;
	while ((*curr > '9' || *curr < '0') && (intptr_t)line - (intptr_t)curr < len) curr = &curr[1]; // Going to first digit
	for (int i = 0; i < race_count; ++i) race_records[i] = atoi(curr);
	  //

	  // - Freeing
	free(line);
	fclose(file);
	//
	
	int mult = 1;
	for (int i = 0; i < race_count; ++i) {
		// Getting min and max time to REACH record
		float min_record, max_record;
		record_hold_time(race_times[i], race_records[i], &min_record, &max_record);

		// Calculating min and max values to BEAT record
		int min = (int)min_record+1; // First integer above min_record
		int max = max_record - (int)max_record > 0? (int)max_record : (int)max_record-1; // First integer below max_record
		//

		mult *= max-min; // Number of ways to beat record
		printf("times that beat race %d's record: %d - %d (possibilities: %d)\n", i, min, max, max-min);
	}

	printf("mult: %d\n", mult);

	return 0;
}

