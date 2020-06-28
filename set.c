#include <stdlib.h>
#include <stdbool.h>
#include "block.h"
#include "set.h"

void create_set(set_t *set) {
    set->blocks_inserted = 0;
    set->blocks = calloc(NUMBER_BLOCKS_PER_SET, sizeof(block_t *));
    for (uint8_t i = 0; i < NUMBER_BLOCKS_PER_SET; i++) {
        set->blocks[i] = calloc(1, sizeof(block_t));
        block_create(set->blocks[i]);
    }
}

void destroy_set(set_t *set) {
    if (set == NULL){
        return;
    }
    for (uint8_t i = 0; i < NUMBER_BLOCKS_PER_SET; i++) {
        block_destroy(set->blocks[i]);
        free(set->blocks[i]);
    }
    free(set->blocks);
}

void set_insert_block(set_t *set, block_t *block, int way, unsigned int tag) {
    block_copy(block, set->blocks[way], tag);
    block_validate(set->blocks[way]);
    block_set_used(block);
    if(!set_is_full(set)) set->blocks_inserted ++;
}

void reset_set(set_t *set) {
    for (int i = 0; i < NUMBER_BLOCKS_PER_SET; i++) {
        block_reset(set->blocks[i]);
    }
}

// block_t* set_get_least_used_block(set_t *set) {
//     block_t* last = NULL;
//     get_way_least_used_block(set);
//     for (size_t i = 0; i < NUMBER_BLOCKS_PER_SET; i++) {
//         if(set->blocks[i] != NULL) {
    
//         }
//     }
// }

bool block_set_is_valid(set_t *set, unsigned int way) {
    return block_is_valid(set->blocks[way]);
}

int search_block_position(set_t *set, uint8_t tag) {
    for (int i = 0; i < NUMBER_BLOCKS_PER_SET; i++) {
        if (block_is_valid(set->blocks[i]) && block_get_tag(set->blocks[i]) == tag) 
            return i;
    }
    return -1; 
}

unsigned char read_block(set_t *set, unsigned int tag, unsigned int offset) {
    int way = search_block_position(set, tag);
    if (way == -1)
        return 0;
    return block_read_byte(set->blocks[way], offset);
}

void set_write_block(set_t *set, unsigned int way, unsigned int offset, char value) {
    block_write_byte(set->blocks[way], offset, value);
}

int get_way_least_used_block(set_t* set) {
    int way = 0;
    int last_counter = set->blocks[0] != NULL ?  set->blocks[0]->lastused : -1;
    if (set->blocks_inserted < NUMBER_BLOCKS_PER_SET) {
        way = set->blocks_inserted - 1;
    } else {
        for (size_t i = 0; i < NUMBER_BLOCKS_PER_SET; i++) {
            if(set->blocks[i] != NULL) {
                if ( set->blocks[i]->lastused > last_counter) {
                    last_counter = set->blocks[i]->lastused;
                    way = i;
                }
            }
        }
    }
    return way;
}

bool set_is_full(set_t* set) {
    return set->blocks_inserted == NUMBER_BLOCKS_PER_SET;
}