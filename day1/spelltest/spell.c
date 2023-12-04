#include <stdio.h>

#define NCOUNT 9
char* numbers[] = {
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine"
};

// Filter string array using check function
//
// strings: array to filter
// len: pointer to the array's lenght (will be altered to the filtered array's length)
// check: function that determines if the string str should be in the filtered array
// check_data: additional data to pass into check function
//
// returns: filetered array
char** filter(char** strings, int* len, bool (*check)(char* str, void* data), void* check_data) {
	int[*len] filtered_ids;

	// Add ids that will be kept to filtered_ids
	int count = 0;
	for (int i = 0; i < len; ++i) {
		if (check(strings[i], check_data)) {
			filtered_ids[count] = strings[i];
			++count;
		}
	}


}

int main(int argc, char* argv[]) {
	if (argc < 2) return 1;

	char* string = argv[1];
	int id = -1;
	for (int i = 0; i < NCOUNT; ++i) {
		if (numbers[i][0] == string[0]) 
			id = i;
			break;
		}
	}

	if (id == -1) printf("none\n");
	else printf("%s\n", numbers[id]);

	return 0;
}

