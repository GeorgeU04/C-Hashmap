#ifndef HASHMAP_H
#define HASHMAP_H
#include <stdbool.h>
#include <stdint.h>

struct Hashmap typedef Hashmap;

Hashmap *init_hashmap(uint32_t size);
const char *get(uint32_t key, Hashmap *map);
bool is_empty(Hashmap *map);
bool lookup(const char *value, Hashmap *map);
void insert(const char *value, Hashmap *map);
void discard(uint32_t key, Hashmap *map);
void discard_by_value(const char *value, Hashmap *map);
void free_hashmap(Hashmap *map);
#endif // !HASHMAP_H
