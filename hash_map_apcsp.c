#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_map.h"

#define MAX_INPUT_LEN 64

void hash_map_print(hash_map_t* hash_map) {
	for (int i = 0; i < hash_map->num_buckets; i++) {
		hash_map_entry_t* entry_ptr = hash_map->buckets[i].head;
		while (entry_ptr) {
			printf("%s : %s\n", entry_ptr->key, entry_ptr->value);
			entry_ptr = entry_ptr->next;
		}
	}
}

bool prompt() {
	printf("\nWould you like to add a key to the HashMap? [Y/n] ");
	char res;

	while ((res = getchar()) == '\n')
		;

	return res != 'n';
}

void scan(char* key, char* value) {
	memset(key, 0, MAX_INPUT_LEN);
	memset(value, 0, MAX_INPUT_LEN);

	printf("Enter a key: ");
	scanf("%s", key);

	printf("Enter a value: ");
	scanf("%s", value);
}

int main() {
	hash_map_t hash_map;
	hash_map_init(&hash_map);

	printf("Inserting dummy values...\n");
	for (int i = 0; i < 9; i++) {
		// 3-letter identifier + iteration# + null byte
		char* key = malloc(5);
		char* value = malloc(5);

		sprintf(key, "key%d", i);
		sprintf(value, "val%d", i);
		hash_map_insert(&hash_map, key, value);
	}

	while (prompt()) {
		char* key = malloc(MAX_INPUT_LEN);
		char* value = malloc(MAX_INPUT_LEN);

		scan(key, value);
		hash_map_insert(&hash_map, key, value);
	}

	printf("\nPrinting HashMap...\n");
	hash_map_print(&hash_map);
}