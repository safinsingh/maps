#include "hash_map.h"
#include <stdio.h>

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
