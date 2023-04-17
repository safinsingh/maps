#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "hash_map.h"
#include "types.h"

void hash_map_init(hash_map_t* hash_map) {
	hash_map_bucket_t empty_bucket;
	empty_bucket.head = NULL;

	hash_map->buckets = (hash_map_bucket_t*)calloc(INIT_BUCKETS, sizeof(hash_map_bucket_t));
	hash_map->num_buckets = INIT_BUCKETS;
}

u64 hash_map_len(hash_map_t* hash_map) {
	u64 len = 0;
	for (int i = 0; i < hash_map->num_buckets; i++) {
		len += hash_map_bucket_len(&hash_map->buckets[i]);
	}
	return len;
}

float hash_map_load_factor(hash_map_t* hash_map) {
	return (float)hash_map_len(hash_map) / hash_map->num_buckets;
}

void hash_map_resize(hash_map_t* hash_map) {
	u64 new_size = (u64)ceil((hash_map_len(hash_map) + 1) / MAGIC_LOAD_FACTOR);
	hash_map_bucket_t* new_buckets =
		(hash_map_bucket_t*)calloc(new_size, sizeof(hash_map_bucket_t));

	for (int i = 0; i < hash_map->num_buckets; i++) {
		hash_map_entry_t* entry_ptr = hash_map->buckets[i].head;

		while (entry_ptr) {
			u64 hash = djb2a(entry_ptr->key);
			hash_map_entry_t* entry =
				hash_map_bucket_insert(&new_buckets[hash % new_size], entry_ptr);
			entry->next = NULL;

			entry_ptr = entry_ptr->next;
		}
	}

	free(hash_map->buckets);
	hash_map->num_buckets = new_size;
	hash_map->buckets = new_buckets;

	DEBUG_PRINT("hash_map_resize() to size %lu with load factor %f\n", new_size,
				hash_map_load_factor(hash_map));
}

hash_map_entry_t* hash_map_insert(hash_map_t* hash_map, char* key, char* value) {
	float load_factor = hash_map_load_factor(hash_map);
	if (fabs(load_factor) > MAGIC_LOAD_FACTOR + EPSILON) {
		hash_map_resize(hash_map);
	}

	hash_map_entry_t* entry = (hash_map_entry_t*)malloc(sizeof(hash_map_entry_t));
	entry->key = key;
	entry->value = value;
	entry->next = NULL;

	u64 hash = djb2a(entry->key);
	u64 bucket = hash % hash_map->num_buckets;

	DEBUG_PRINT("hash_map_insert(%s, %s) into bucket (%lu)\n", entry->key, entry->value, bucket);

	hash_map_bucket_insert(&hash_map->buckets[bucket], entry);
	return entry;
}

hash_map_entry_t* hash_map_get(hash_map_t* hash_map, char* key) {
	u64 hash = djb2a(key);

	hash_map_entry_t* entry_ptr = hash_map->buckets[hash % hash_map->num_buckets].head;

	while (entry_ptr) {
		if (strlen(key) == strlen(entry_ptr->key) && strcmp(key, entry_ptr->key) == 0) {
			return entry_ptr;
		}
		entry_ptr = entry_ptr->next;
	}
	return NULL;
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

u64 hash_map_bucket_collisions(hash_map_t* hash_map) {
	u64 collisions = 0;
	for (int i = 0; i < hash_map->num_buckets; i++) {
		if (hash_map_bucket_len(&hash_map->buckets[i]) > 1)
			collisions++;
	}
	return collisions;
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
