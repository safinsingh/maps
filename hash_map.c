#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long u64;

#define INIT_BUCKETS 7
#define MAGIC_LOAD_FACTOR 0.7
#define DJB2A_MAGIC 5381
#define EPSILON 0.10

u64 djb2a(char* str) {
	u64 hash = DJB2A_MAGIC;
	while (*str) {
		hash = ((hash << 5) + hash) ^ *str++;
	}
	return hash;
}

typedef struct hash_map_entry {
	struct hash_map_entry* next;
	char* key;
	char* value;
} hash_map_entry_t;

typedef struct {
	hash_map_entry_t* head;
} hash_map_bucket_t;

typedef struct {
	hash_map_bucket_t* buckets;
	u64 num_buckets;
} hash_map_t;

void hash_map_init(hash_map_t* hash_map) {
	hash_map_bucket_t empty_bucket;
	empty_bucket.head = NULL;

	hash_map->buckets = calloc(INIT_BUCKETS, sizeof(hash_map_bucket_t));
	hash_map->num_buckets = INIT_BUCKETS;
}

u64 hash_map_bucket_len(hash_map_bucket_t* bucket) {
	u64 len = 0;
	hash_map_entry_t* entry_ptr = bucket->head;

	while (entry_ptr) {
		len++;
		entry_ptr = entry_ptr->next;
	}

	return len;
}

u64 hash_map_len(hash_map_t* hash_map) {
	u64 len = 0;
	for (int i = 0; i < hash_map->num_buckets; i++) {
		len += hash_map_bucket_len(&hash_map->buckets[i]);
	}
	return len;
}

u64 hash_map_bucket_collisions(hash_map_t* hash_map) {
	u64 collisions = 0;
	for (int i = 0; i < hash_map->num_buckets; i++) {
		if (hash_map_bucket_len(&hash_map->buckets[i]) > 1)
			collisions++;
	}
	return collisions;
}

float hash_map_load_factor(hash_map_t* hash_map) {
	return (float)hash_map_len(hash_map) / hash_map->num_buckets;
}

hash_map_entry_t* hash_map_bucket_insert(hash_map_bucket_t* hash_map_bucket,
										 hash_map_entry_t* entry) {
	hash_map_entry_t** head = &hash_map_bucket->head;
	hash_map_entry_t* entry_ptr = *head;

	if (!entry_ptr) {
		*head = entry;
	} else {
		while (entry_ptr->next) {
			entry_ptr = entry_ptr->next;
		}
		entry_ptr->next = entry;
	}

	return entry;
}

void hash_map_resize(hash_map_t* hash_map, u64 size) {
	hash_map_bucket_t* new_buckets = calloc(size, sizeof(hash_map_bucket_t));

	for (int i = 0; i < hash_map->num_buckets; i++) {
		hash_map_entry_t* entry_ptr = hash_map->buckets[i].head;

		while (entry_ptr) {
			u64 hash = djb2a(entry_ptr->key);
			hash_map_entry_t* entry =
				hash_map_bucket_insert(&new_buckets[hash % size], entry_ptr);
			entry->next = NULL;

			entry_ptr = entry_ptr->next;
		}
	}

	free(hash_map->buckets);
	hash_map->num_buckets = size;
	hash_map->buckets = new_buckets;
}

hash_map_entry_t* hash_map_insert(hash_map_t* hash_map,
								  char* key,
								  char* value) {
	float load_factor = hash_map_load_factor(hash_map);
	if (fabs(load_factor) > MAGIC_LOAD_FACTOR + EPSILON) {
		hash_map_resize(hash_map, load_factor);
	}

	hash_map_entry_t* entry = malloc(sizeof(hash_map_entry_t));
	entry->key = key;
	entry->value = value;
	entry->next = NULL;

	u64 hash = djb2a(entry->key);
	hash_map_bucket_insert(&hash_map->buckets[hash % hash_map->num_buckets],
						   entry);
	return entry;
}

hash_map_entry_t* hash_map_get(hash_map_t* hash_map, char* key) {
	u64 hash = djb2a(key);

	hash_map_entry_t* entry_ptr =
		hash_map->buckets[hash % hash_map->num_buckets].head;

	while (entry_ptr) {
		if (strlen(key) == strlen(entry_ptr->key) &&
			strcmp(key, entry_ptr->key) == 0) {
			return entry_ptr;
		}
		entry_ptr = entry_ptr->next;
	}
	return NULL;
}

int main() {
	hash_map_t hash_map;
	hash_map_init(&hash_map);
	hash_map_insert(&hash_map, "name1", "safin1");
	hash_map_insert(&hash_map, "name2", "safin2");
	hash_map_insert(&hash_map, "name3", "safin3");
	hash_map_insert(&hash_map, "name4", "safin4");
	hash_map_insert(&hash_map, "name5", "safin5");
	hash_map_insert(&hash_map, "name6", "safin6");

	hash_map_entry_t* entry = hash_map_get(&hash_map, "name1");
	printf("Key/value pair: %s - %s\n", entry->key, entry->value);

	u64 len = hash_map_len(&hash_map);
	printf("Length: %lu\n", len);

	float load_factor = hash_map_load_factor(&hash_map);
	printf("Load factor: %f\n", load_factor);

	hash_map_insert(&hash_map, "name7", "safin7");
	hash_map_insert(&hash_map, "name8", "safin8");
	printf("Buckets: %lu\n", hash_map.num_buckets);

	// retrieval still works after resize!
	hash_map_entry_t* entry2 = hash_map_get(&hash_map, "name2");
	printf("Key/value pair: %s - %s\n", entry2->key, entry2->value);

	u64 collisions = hash_map_bucket_collisions(&hash_map);
	printf("Bucket collisions: %lu\n", collisions);

	load_factor = hash_map_load_factor(&hash_map);
	printf("Load factor: %f\n", load_factor);
}