#ifndef HASH_MAP_H
#define HASH_MAP_H

#define INIT_BUCKETS 13
#define MAGIC_LOAD_FACTOR 0.7
#define DJB2A_MAGIC 5381
#define EPSILON 0.05

typedef unsigned long u64;

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

u64 djb2a(char* str);

void hash_map_init(hash_map_t* hash_map);
u64 hash_map_len(hash_map_t* hash_map);
float hash_map_load_factor(hash_map_t* hash_map);
void hash_map_resize(hash_map_t* hash_map);
hash_map_entry_t* hash_map_insert(hash_map_t* hash_map, char* key, char* value);
hash_map_entry_t* hash_map_get(hash_map_t* hash_map, char* key);

u64 hash_map_bucket_len(hash_map_bucket_t* bucket);
u64 hash_map_bucket_collisions(hash_map_t* hash_map);
hash_map_entry_t* hash_map_bucket_insert(hash_map_bucket_t* hash_map_bucket,
										 hash_map_entry_t* entry);

#endif /* !HASH_MAP_H */