#include <stdlib.h>
#include <stdint.h>
#include "cache.h"

void create_cache(memory_t *pri_mem){
    cache->pri_mem = pri_mem;
    cache->sets = calloc(NUMBER_SETS, sizeof(set_t *));
    for (int i = 0; i < NUMBER_SETS; i++) {
        cache->sets[i] = calloc(1, sizeof(set_t));
        set_create(cache->sets[i]);
    }
    cache->hits = 0;
    cache->miss = 0;
}

/**********************************************************************
    Get the 6th lastest bits. 2^6 = 128 = Block Size
    So if we have:
    1010101010101010
    we want the bits inside "|"
    101010101|0101010|
    So we have to shift to left 10 bits adding zeros to right:
    |0101010|0000000000
    And then we have to shift to right again adding zeros to left:
    000000000|0101010|

************************************************************************/
unsigned int get_offset(unsigned int address) {
    uint16_t offset = (address << 9);
    return offset >> 9;
}

/**********************************************************************
    We have to get the 7 bits after the 6 offset bits.
    Because 2^3 = 8 = Number of sets.
    So if we have:
    1010101010101010
    We want the bits inside "|"
    1010101|010   |0101010
   ^tag    |^index|^offset

    So we have to shift to left 6 times adding zeros to right <<:
    |010|0101010000000

    And then, we have to shift 7 places to right adding zeros to left >>:
    0000000000000|010|
************************************************************************/
unsigned int find_set(unsigned int address) {
    uint16_t set = (address << 6);
    return set >> 7;
}

/**********************************************************************
    We have to get the first 6 bits
    So if we have:
    1010101010101010
    We want the bits inside "|"
    1010101|010   |101010
   ^tag    |^index|^offset

    And then, we have to shift 9 places to right adding zeros to left >>:
    000000000|0101010|
************************************************************************/

uint16_t get_tag(unsigned int address) {
    return (address >> 10);
}

unsigned int select_oldest(unsigned int setnum) {
    return set_select_oldest(cache->sets[setnum]);
}

void init() {
    for (int i = 0; i < NUMBER_SETS; i++) {
        set_reset(cache->sets[i]);
    }
}

void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set) {
    if (way >= NUMBER_BLOCKS_PER_SET || set >= NUMBER_SETS)
        return;
    block_t *block = memory_get_block(cache->pri_mem, blocknum);
    unsigned int tag = (blocknum >> 3);
    set_insert_block(cache->sets[set], block, way, tag);
    set_update_counters(cache->sets[set], way);
}


unsigned int get_block_num(unsigned int address) {
    return (address >> 7);
}

unsigned char read_byte(unsigned int address) {
    address &= 0xFFFF; // Addresses are 16 bit long
    unsigned int set = find_set(address);
    unsigned int offset = get_offset(address);
    uint16_t tag = get_tag(address);
    unsigned int way = set_get_block_position(cache->sets[set], tag);
    if (way == NUMBER_BLOCK_INVALIDA) {
        cache->miss++;
        // Traigo el que necesito de memoria
        way = set_select_oldest(cache->sets[set]);
        read_tocache(get_block_num(address),way,set);
    } else {
        cache->hits++;
    }
    return set_read_block(cache->sets[set], way ,offset);
}


void write_byte(unsigned int address, unsigned char value) {
    address &= 0xFFFF;
    uint16_t tag = get_tag(address);
    unsigned int set = find_set(address);
    unsigned int offset = get_offset(address);
    unsigned int way = set_get_block_position(cache->sets[set], tag);
    if (way == NUMBER_BLOCK_INVALIDA) {
        cache->miss++;    
    } else {
        cache->hits++;
        set_write_block(cache->sets[set], way, offset, value);
        set_update_counters(cache->sets[set], way);
    }
    memory_write_byte(cache->pri_mem,address,value);        
}

float get_miss_rate() {
    return (cache->miss / (float)(cache->miss + cache->hits));
}


void destroy_cache() {
    for (int i = 0; i < NUMBER_SETS; i++){
        set_destroy(cache->sets[i]);
        free(cache->sets[i]);
    }
    free(cache->sets);
}