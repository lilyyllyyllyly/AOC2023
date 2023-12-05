#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define RED_AMOUNT   12
#define GREEN_AMOUNT 13
#define BLUE_AMOUNT  14

struct set {
	long red;
	long green;
	long blue;
};

long set_power(struct set set) {
	return set.red * set.green * set.blue;
}

// returns NULL in case of error
struct set* create_set(char* setstr, int len, char** end) {
	struct set* set = calloc(1, sizeof(*set));

	int i = 0;
	while (i < len && setstr[i] != ';') {
		// Getting amount
		for (; i < len && (setstr[i] > '9' || setstr[i] < '0'); ++i) ; // Going to first digit, or to end of string
		if (i >= len) break; // No digits

		char* end_number = NULL;
		long amount = strtol(&setstr[i], &end_number, 10);
		if (end_number == NULL) { // setstr[i] -> end of setstr are all digits. Missing color.
			fprintf(stderr, "WARNING: one of the sets is missing a color.\n");
			return NULL;
		}
		i += (intptr_t)end_number - (intptr_t)&setstr[i]; // Moving i to first character that isnt a digit
		// ---

		// Getting color
		for (; i < len && (setstr[i] < 'a' || setstr[i] > 'z'); ++i) ; // Going to first letter, or to end of string
		if (i >= len) { // No color
			fprintf(stderr, "WARNING: one of the sets is missing a color.\n");
			return NULL;
		}
		
		int start_color = i;
		for (; i < len && (setstr[i] != ',' && setstr[i] != ';'); ++i) ; // Going to the end of the color
		int size_color = i - start_color;
		char* color = calloc(size_color + 1, sizeof(char)); // + 1 for null terminator
		memcpy(color, &setstr[start_color], size_color);
		// ---

		if (strcmp(color, "red") == 0) {
			set->red = amount;
		} else if (strcmp(color, "green") == 0) {
			set->green = amount;
		} else if (strcmp(color, "blue") == 0) {
			set->blue = amount;
		} else {
			fprintf(stderr, "WARNING: one of the sets has an invalid color.\n");
			return NULL;
		}

		free(color);
	}

	if (++i < len) *end = &setstr[i];
	else *end = NULL;
	return set;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "ERROR: games file not specified!\n");
		return 1;
	}

	FILE* file = fopen(argv[1], "r");
	if (!file) {
		fprintf(stderr, "ERROR: could not open file %s.\n", argv[1]);
		return 1;
	}

	size_t alloc = sizeof(char);
	char* line = malloc(alloc);
	int len = 0;
	long sum = 0;
	long power_sum = 0;
	while((len = getline(&line, &alloc, file)) != -1) {

		// Getting gameid
		int i = 0;
		for (; i < len && (line[i] > '9' || line[i] < '0'); ++i) ; // Going to first digit, or to end of string
		if (i >= len) continue; // No digits
		char* end_gameid;
		long gameid = strtol(&line[i], &end_gameid, 10);
		if (end_gameid == NULL) { // line[i] -> end of line are all digits. Missing set.
			fprintf(stderr, "WARNING: one of the games has no sets.\n");
			continue;
		}
		i += (intptr_t)end_gameid - (intptr_t)&line[i]; // Moving i to first character that isnt a digit
		// ---
		
		printf("GAME %ld:\n", gameid);

		struct set max_set = { .red = 0, .green = 0, .blue = 0 };
		char* start = &line[i];
		char* end = NULL;
		do {
			struct set* set = create_set(start, strlen(start)-1, &end);
			if (!set) break;

			printf("\tred: %ld - green: %ld - blue: %ld\n", set->red, set->green, set->blue);

			if (set->red   > max_set.red)   max_set.red   = set->red;
			if (set->green > max_set.green) max_set.green = set->green;
			if (set->blue  > max_set.blue)  max_set.blue  = set->blue;

			free(set);
			start = end;
		} while (end != NULL);
		
		int possible = max_set.red <= RED_AMOUNT && max_set.green <= GREEN_AMOUNT && max_set.blue <= BLUE_AMOUNT;
		if (possible) sum += gameid;
		power_sum += set_power(max_set);

		printf("max | red: %ld - green: %ld - blue: %ld\n", max_set.red, max_set.green, max_set.blue);
		printf("possible: %s\n------------------\n", possible? "true" : "false");
	}

	printf("\nSUM: %ld\n", sum);
	printf("POWER SUM: %ld\n", power_sum);

	free(line);
	fclose(file);
	return 0;
}

