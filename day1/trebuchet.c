#include <stdio.h>
#include <stdlib.h>

#include "spell.h"

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
	int len = 0;
	int sum = 0;
	while ((len = getline(&line, &alloc, file)) != -1) {
		char* number = calloc(3, sizeof(char));

		// Going through the characters left to right to find the first digit
		for (int i = 0; i < len; ++i) {
			number[0] = line[i];
			if (number[0] <= '9' && number[0] >= '0') { // Current char is a digit
				break;
			} else { // If its not, see if its the start of a spelled out number
				int spelled_number = find_number(&line[i]);
				if (spelled_number != -1) { // It is!
					number[0] = '0' + spelled_number;
					break;
				}
			}
		}

		if (number[0] < '0' || number[0] > '9') { // No digits
			goto free;
		}

		// Going through the characters right to left to find the last digit
		for (int i = len-1; i >= 0; --i) {
			number[1] = line[i];
			if (number[1] <= '9' && number[1] >= '0') { // Current char is a digit
				break;
			} else { // If its not, see if its the start of a spelled out number
				int spelled_number = find_number(&line[i]);
				if (spelled_number != -1) { // It is!
					number[1] = '0' + spelled_number;
					break;
				}
			}
		}

		printf("%s^ %s\n", line, number);
		sum += atoi(number);

free:
		free(number);
	}
	free(line);
	fclose(file);

	printf("sum: %d\n", sum);

	return 0;
}

