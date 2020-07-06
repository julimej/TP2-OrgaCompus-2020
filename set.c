#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "block.h"
#include "set.h"

void set_create(set_t *set) {
    set->blocks_inserted = 0;
    set->blocks = calloc(NUMBER_BLOCKS_PER_SET, sizeof(block_t *));
    for (uint8_t i = 0; i < NUMBER_BLOCKS_PER_SET; i++) {
        set->blocks[i] = calloc(1, sizeof(block_t));
        block_create(set->blocks[i]);
    }
}

void set_destroy(set_t *set) {
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

void set_reset(set_t *set) {
    for (int i = 0; i < NUMBER_BLOCKS_PER_SET; i++) {
        block_reset(set->blocks[i]);
    }
}

int set_get_block_position(set_t *set, uint8_t tag) {
    // printf("Buscando tag %d\n", tag);
    for (int i = 0; i < NUMBER_BLOCKS_PER_SET; i++) {
        if (block_is_valid(set->blocks[i]) && block_get_tag(set->blocks[i]) == tag) {
            // printf("block iter %d tag %d\n",i, block_get_tag(set->blocks[i]));
            return i;
        }
    }
    return NUMBER_BLOCK_INVALIDA; 
}

unsigned char set_read_block(set_t *set, unsigned int way,unsigned int offset) {
    return block_read_byte(set->blocks[way], offset);
}

void set_write_block(set_t *set, unsigned int way, unsigned int offset, char value) {
    block_write_byte(set->blocks[way], offset, value);
}

unsigned int set_select_oldest(set_t* set) {
    int way = 0;
    int last_counter = 0;
    if (set->blocks_inserted < NUMBER_BLOCKS_PER_SET) {
        way = set->blocks_inserted;
    } else {
        for (size_t i = 0; i < NUMBER_BLOCKS_PER_SET; i++) {
            if (block_is_valid(set->blocks[i]) && set->blocks[i]->lastused > last_counter) {
                last_counter = set->blocks[i]->lastused;
                way = i;
            }
        }
    }
    return way;
}

bool set_is_full(set_t* set) {
    return set->blocks_inserted == NUMBER_BLOCKS_PER_SET;
}

void set_update_counters(set_t* set, unsigned int last_used) {
    for (size_t i = 0; i < NUMBER_BLOCKS_PER_SET; i++) {
        if (block_is_valid(set->blocks[i])){
            if (i == last_used) {
                block_set_used(set->blocks[i]);
            } else {
                block_add_not_used(set->blocks[i]);
            }
            // printf("Bloque %d usado %d veces\n", i , set->blocks[i]->lastused);
        }
    }
}