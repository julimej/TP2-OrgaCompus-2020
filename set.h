#ifndef SET_H
#define SET_H
#define NUMBER_BLOCKS_PER_SET 4
#include <stdint.h>
#include "block.h"
#include "cola.h"

struct set {
  block_t **blocks;
  int blocks_inserted;
};

typedef struct set set_t;

void create_set(set_t *set);

void destroy_set(set_t *set);

void set_insert_block(set_t *set, block_t *block, int way, unsigned int tag);

void reset_set(set_t *set);

block_t* set_get_least_used_block(set_t *set);

int search_block_position(set_t *set, uint8_t tag);

unsigned char read_block(set_t *set, unsigned int tag, unsigned int offset);

void set_write_block(set_t *set, unsigned int way, unsigned int offset, char value);

int get_way_least_used_block(set_t *set);

bool set_is_full(set_t* set);

int set_get_way_to_insert(set_t* set);

#endif