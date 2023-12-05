#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// check if str is less than or equal to the size pointed to by data
int check_lenle(char* str, void* data) {
	return strlen(str) <= *(size_t*)(data);
}

// check if str starts with the char pointed to by data
int check_firstc(char* str, void* data) {
	return str[0] == *(char*)(data);
}

// Filter string array using check function
//
// strings: array to filter (will be altered)
// len: pointer to the array's lenght (will be altered to the filtered array's length)
// check: function that determines if the string str should be in the filtered array
// check_data: additional data to pass into check function
char** filter(char* strings[], int* len, int (*check)(char* str, void* data), void* check_data, int offset) {
	char** filtered_strings = calloc(*len, sizeof(char*));

	// Add ids that will be kept to filtered_ids
	int count = 0;
	for (int i = 0; i < *len; ++i) {
		if (offset > (int)strlen(strings[i])) continue;

		if (check(&strings[i][offset], check_data)) {
			filtered_strings[count] = strings[i];
			++count;
		}
	}
	*len = count;

	return filtered_strings;
}

char** string_in_list(char* string, char* list[], int* len, int offset) {
	// All elements left in matches are bigger than the string (none can possibly match)
	if (offset > (int)strlen(string)) {
		*len = 0;
		return NULL;
	}

	char** new_list = filter(list, len, check_firstc, (void*)(&string[offset]), offset);

	if (*len <= 1) { // Match found or doesnt exist
		return new_list;
	}

	// Still multiple matches, try next character
	char** next = string_in_list(string, new_list, len, offset+1);
	free(new_list);
	return next;
}

int main(int argc, char* argv[]) {
	if (argc < 2) return 1;

	char* string = argv[1];
	size_t input_len = strlen(string);

	int len = NCOUNT;
	char** nums_inlen = filter(numbers, &len, check_lenle, (void*)&input_len, 0);

	int offset = 0;
	char** possible_nums = string_in_list(string, nums_inlen, &len, offset);

	if (possible_nums[0]) {
		puts(possible_nums[0]);
	}

	free(possible_nums);
	free(nums_inlen);

	return 0;
}

