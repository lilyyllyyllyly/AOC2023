#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// Returns the 2 possible needed hold times (by parameter, min_out and max_out) to REACH the record (not pass it) given the max race time and record time
void record_hold_time(long max, long record, double* min_out, double* max_out) {
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
	char* number = NULL;
	int len, num_len;
	long race_time, race_record;

	// Reading file
	  // - Get race time number
	len = getline(&line, &alloc, file);

	number = malloc(len * sizeof(char));
	num_len = 0;
	for (int i = 0; i < len; ++i) {
		if (line[i] > '9' || line[i] < '0') continue;
		number[num_len++] = line[i];
	}
	if (num_len < len-1) number[num_len] = '\0';
	
	race_time = strtol(number, NULL, 10);

	  // - Get record number
	len = getline(&line, &alloc, file);

	num_len = 0;
	for (int i = 0; i < len; ++i) {
		if (line[i] > '9' || line[i] < '0') continue;
		number[num_len++] = line[i];
	}
	if (num_len < len-1) number[num_len] = '\0';

	race_record = strtol(number, NULL, 10);

	  // - Freeing
	free(line);
	free(number);
	fclose(file);
	//
	
	// Getting min and max time to REACH record
	double min_record, max_record;
	record_hold_time(race_time, race_record, &min_record, &max_record);

	// Calculating min and max values to BEAT record
	long min = (long)min_record+1; // First integer above min_record
	long max = max_record - (long)max_record > 0? (long)max_record : (long)max_record-1; // First integer below max_record
	//

	printf("times that beat the race record: %ld - %ld (possibilities: %ld)\n", min, max, max-min + 1);

	return 0;
}

