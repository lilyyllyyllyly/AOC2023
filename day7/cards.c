#include <stdio.h>
#include <string.h>

#define HAND_SIZE   5
#define CARD_COUNT 13

struct card_group {
	char type;
	int count;
};

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "ERROR: no hands file provided!\n");
		return 1;
	}

	int type_count = 0;
	struct card_group groups[HAND_SIZE];

	char* curr = argv[1];
	for (int i = 0; i < HAND_SIZE; ++i) {
		if (strchr(argv[1], *curr) == curr) { // This is the first occurance of this card type
			groups[type_count++] = (struct card_group){ *curr, 1 };
		} else {
			for (int i = 0; i < type_count; ++i) {
				if (groups[i].type == *curr) {
					++groups[i].count;
					break;
				}
			}
		}
		curr = &curr[1];
	}

	printf("%d\n", type_count);
	for (int i = 0; i < type_count; ++i) printf("type: %c | count: %d\n", groups[i].type, groups[i].count);

	return 0;
}

