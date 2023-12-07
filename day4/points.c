#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "ERROR: cards file not specified!\n");
		return 1;
	} else if (argc < 3) {
		fprintf(stderr, "ERROR: winning numbers count not specified!");
	} else if (argc < 4) {
		fprintf(stderr, "ERROR: elf's numbers count not specified!");
	}

	FILE* file = fopen(argv[1], "r");
	if (!file) {
		fprintf(stderr, "ERROR: could not open file %s.\n", argv[1]);
		return 1;
	}
	const int win_count = atoi(argv[2]), elf_count = atoi(argv[3]);

	long* win_ns = malloc(win_count * sizeof(long));
	long* elf_ns = malloc(elf_count * sizeof(long));

	size_t alloc = sizeof(char);
	char* line = malloc(alloc);
	int len;

	int total = 0;
	while ((len = getline(&line, &alloc, file)) != -1) {
		char* curr = line;

		// Moving curr after :
		for (; (intptr_t)curr - (intptr_t)line < len && *curr != ':'; curr = &curr[1]) ;
		if ((intptr_t)curr - (intptr_t)line >= len) {
			fprintf(stderr, "WARNING: No colon on one of the cards. Ignoring.\n");
			continue;
		}
		if ((intptr_t)curr - (intptr_t)line + 1 >= len) {
			fprintf(stderr, "WARNING: No winning numbers on one of the cards. Ignoring.\n");
			continue;
		}
		curr = &curr[1];

		// Filling win_ns
		for (int i = 0; curr != NULL && i < win_count; ++i) {
			win_ns[i] = strtol(curr, &curr, 10); // read number to win_ns and move curr forward
		}

		// Moving curr to next number
		for (; (intptr_t)curr - (intptr_t)line < len && (*curr > '9' || *curr < '0'); curr = &curr[1]) ;
		if ((intptr_t)curr - (intptr_t)line >= len) {
			fprintf(stderr, "WARNING: No elf numbers on one of the cards. Ignoring.\n");
			continue;
		}

		// Filling elf_ns
		for (int i = 0; curr != NULL && i < elf_count; ++i) {
			char* end = NULL;
			elf_ns[i] = strtol(curr, &end, 10); // read number to win_ns and move curr forward
			curr = end;
		}
		//
		
		int count = 0;
		for (int i = 0; i < elf_count; ++i) {
			int is_winning = 0;

			for (int j = 0; j < win_count; ++j) {
				is_winning = elf_ns[i] == win_ns[j];
				if (is_winning) break;
			}

			if (is_winning) ++count;
		}

		int points = 0;
		if (count > 0) {
			++points;
			points <<= (count-1);
		}
		total += points;
	}

	printf("Total points: %d\n", total);

	free(win_ns);
	free(elf_ns);
	free(line);
	fclose(file);
	return 0;
}

