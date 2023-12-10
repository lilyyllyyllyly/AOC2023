#include <stdlib.h>
#include <string.h>

#define NCOUNT 9
static char* numbers[] = {
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
static int check_lenle(char* str, void* data) {
	return strlen(str) <= *(size_t*)(data);
}

// check if str starts with the char pointed to by data
static int check_firstc(char* str, void* data) {
	return str[0] == *(char*)(data);
}

// Filter string array using check function
//
// strings: array to filter
// len: pointer to the array's lenght (will be altered to the filtered array's length)
// check: function that determines if the string str should be in the filtered array
// check_data: additional data to pass into check function
// returns: strings left after filtering
static char** filter(char* strings[], int* len, int (*check)(char* str, void* data), void* check_data, int offset) {
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

static char** string_in_list(char* string, char* list[], int* len, int offset) {
	// All elements left in matches are bigger than the string (none can possibly match)
	if (offset > (int)strlen(string)) {
		*len = 0;
		return NULL;
	}

	char** new_list = filter(list, len, check_firstc, (void*)(&string[offset]), offset);

	if (*len == 0 || (*len == 1 && offset >= (int)strlen(new_list[0])-1)) { // Match doesnt exist or was found
		return new_list;
	}

	// Still multiple matches, try next character
	char** next = string_in_list(string, new_list, len, offset+1);
	free(new_list);
	return next;
}

// returns -1 if not found
static int find_index(void* element, void* collection[], int len) {
	for (int i = 0; i < len; ++i) {
		if (element == collection[i]) return i;
	}

	return -1;
}

// Check for a spelled out number in the start of string.
// If found, returns the integer representation of the number.
// Otherwise, returns -1.
int find_number(char* string) {
	int ret = -1;
	int count = NCOUNT;
	size_t str_len = strlen(string);

	char** nums_inlen = filter(numbers, &count, check_lenle, (void*)&str_len, 0);
	if (count <= 1) {
		if (nums_inlen[0]) {
			ret = find_index((void*)nums_inlen[0], (void**)numbers, NCOUNT) + 1;
		}
		goto skip;
	}

	int offset = 0;
	char** possible_nums = string_in_list(string, nums_inlen, &count, offset);

	if (possible_nums) {
		if (possible_nums[0]) {
			ret = find_index((void*)possible_nums[0], (void**)numbers, NCOUNT) + 1;
		}
		free(possible_nums);
	}

skip:
	free(nums_inlen);
	return ret;
}

