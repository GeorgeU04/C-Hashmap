#include "hashmap.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

struct Hashmap {
  uint32_t num_of_elements;
  uint32_t size;
  char (*array)[64];
};

// generates key using DJB2 algorithm
uint64_t gen_key(const char *value, Hashmap *map) {
  uint64_t hash = 5381;
  int32_t c;
  while ((c = *value++))
    hash = ((hash << 5) + hash) + c;
  return hash % map->size;
}

// initializes the hashmap
Hashmap *init_hashmap(uint32_t size) {
  Hashmap *hashmap = malloc(sizeof(Hashmap));
  if (!hashmap) {
    fprintf(stderr, "[Error]: Memory Allocation Failed\n");
    exit(EXIT_FAILURE);
  }
  hashmap->size = size;
  hashmap->num_of_elements = 0;
  hashmap->array = malloc(size * sizeof(*hashmap->array));
  if (!hashmap->array) {
    fprintf(stderr, "[Error]: Memory Allocation Failed\n");
    free(hashmap);
    exit(EXIT_FAILURE);
  }
  // Initialize the array to empty strings
  for (size_t i = 0; i < size; i++) {
    hashmap->array[i][0] = '\0';
  }
  return hashmap;
}

// takes in a key and returns the item if one exists at key
const char *get(uint32_t key, Hashmap *map) {
  if (key >= map->size)
    return "INVALID KEY";
  if (map->array[key][0] == '\0')
    return "Empty Key";
  return map->array[key];
}

// takes in a value and returns if it is in the hashmap
bool lookup(const char *value, Hashmap *map) {
  uint64_t key = gen_key(value, map);
  size_t original_key = key;
  while (map->array[key][0] != '\0') {
    if (strcmp(map->array[key], value) == 0)
      return true;
    key = (key + 1) % map->size;
    if (key == original_key)
      break;
  }
  return false;
}

// checks if it is empty
bool is_empty(Hashmap *map) { return map->num_of_elements == 0; }

// inserts item into hashmap and does simple collision checking by moving up one
void insert(const char *value, Hashmap *map) {
  if (map->num_of_elements >= map->size) {
    fprintf(stderr, "Hashmap is full\n");
    return;
  }

  uint64_t key = gen_key(value, map);
  size_t original_key = key;

  // Check if the item is already in the hashmap
  while (map->array[key][0] != '\0') {
    if (strcmp(value, map->array[key]) == 0) {
      fprintf(stderr, "Item is already in Hashmap\n");
      return;
    }
    key = (key + 1) % map->size;
    if (key == original_key) {
      fprintf(stderr, "Hashmap is full\n");
      return;
    }
  }

  strncpy(map->array[key], value, 63);
  map->array[key][63] = '\0';
  map->num_of_elements++;
}

// removes value at a certain key
void discard(uint32_t key, Hashmap *map) {
  if (is_empty(map)) {
    fprintf(stderr, "Hashmap is empty\n");
    return;
  }
  if (key >= map->size) {
    fprintf(stderr, "Key is out of bounds\n");
    return;
  }
  if (map->array[key][0] != '\0') {
    map->array[key][0] = '\0';
    --map->num_of_elements;
  } else {
    fprintf(stderr, "Nothing at key %d\n", key);
  }
}

// removes value by looking for it in hashmap
void discard_by_value(const char *value, Hashmap *map) {
  if (is_empty(map)) {
    fprintf(stderr, "Hashmap is empty\n");
    return;
  }

  uint64_t key = gen_key(value, map);
  size_t original_key = key;

  while (1) {
    if (map->array[key][0] == '\0') {
      fprintf(stderr, "Item is not in Hashmap\n");
      return;
    }

    if (strcmp(value, map->array[key]) == 0) {
      map->array[key][0] = '\0';
      --map->num_of_elements;
      return;
    }

    key = (key + 1) % map->size;

    if (key == original_key) {
      fprintf(stderr, "Item is not in Hashmap\n");
      return;
    }
  }
}

// frees memory
void free_hashmap(Hashmap *map) {
  free(map->array);
  free(map);
}
