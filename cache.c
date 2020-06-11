#include <stdlib.h>
#include <stdint.h>
#include "cache.h"

void create_cache(memory_t *pri_mem){
    cache->pri_mem = pri_mem;
    cache->sets = calloc(NUMBER_SETS, sizeof(set_t *));
    for (int i = 0; i < NUMBER_SETS; i++) {
        cache->sets[i] = calloc(1, sizeof(set_t));
        create_set(cache->sets[i]);
    }
    cache->hits = 0;
    cache->miss = 0;
}

/**********************************************************************
    Get the 6th lastest bits. 2^6 = 64 = Block Size
    So if we have:
    1010101010101010
    we want the bits inside "|"
    1010101010|101010|
    So we have to shift to left 10 bits adding zeros to right:
    |101010|0000000000
    And then we have to shift to right again adding zeros to left:
    0000000000|101010|

************************************************************************/
unsigned int get_offset(unsigned int address) {
    uint16_t offset = (address << 10);
    return offset >> 10;
}

/**********************************************************************
    We have to get the 7 bits after the 6 offset bits.
    Because 2^7 = 128 = Number of sets.
    So if we have:
    1010101010101010
    We want the bits inside "|"
    101|0101010|101010
   ^tag| ^index|^offset

    So we have to shift to left 3 times adding zeros to right <<:
    |0101010|101010000

    And then, we have to shift 9 places to right adding zeros to left >>:
    000000000|0101010|
************************************************************************/
unsigned int find_set(unsigned int address) {
    uint16_t set = (address << 3);
    return set >> 9;
}

/**********************************************************************
    We have to get the first 3 bits.
    Because 2^7 = 128.
    So if we have:
    1010101010101010
    We want the bits inside "|"
    101|0101010|101010
   ^tag| ^index|^offset

    So we have to shift to left 3 times adding zeros to right <<:
    |0101010|101010000

    And then, we have to shift 9 places to right adding zeros to left >>:
    000000000|0101010|
************************************************************************/

uint16_t get_tag(unsigned int address) {
    return (address >> 12);
}


void destroy_cache()
{
    for (int i = 0; i < NUMBER_SETS; i++)
    {
        destroy_set(cache->sets[i]);
        free(cache->sets[i]);
    }
    free(cache->sets);
}

void init() {
    for (int i = 0; i < NUMBER_SETS; i++) {
        reset_set(cache->sets[i]);
    }
}

void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set) {
    if (way >= NUMBER_BLOCKS_PER_SET || set >= NUMBER_SETS)
        return;
    block_t *block = memory_get_block(cache->pri_mem, blocknum);
    unsigned int tag = (blocknum >> 7);
    set_insert_block(cache->sets[set], block, way, tag);
}


unsigned int get_block_num(unsigned int address)
{
    return (address >> 6);
}

void write_tocache(unsigned int address, unsigned char value) {
    unsigned int set = find_set(address);
    unsigned int offset = get_offset(address);
    uint16_t tag = get_tag(address);
    unsigned int way = search_block_position(cache->sets[set], tag);
    set_write_block(cache->sets[set], way, offset, value);
}

unsigned char read_byte(unsigned int address) {
    address &= 0xFFFF; // Addresses are 16 bit long
    unsigned int set = find_set(address);
    unsigned int offset = get_offset(address);
    uint16_t tag = get_tag(address);
    unsigned int way = search_block_position(cache->sets[set], tag);
    if (search_block_position(cache->sets[set], tag) == -1) {
        cache->miss++;
        // Si no esta, lo traigo de memoria y devuelvo el dato despues
        block_t *last = set_get_last_block(cache->sets[set]);
        if (last != NULL) {
            // Me fijo si el que tengo reemplazar, esta dirty o no, si esta lo escribo a memory
            if (block_is_dirty(last)) {
                memory_write_block(cache->pri_mem, last, address, tag);
            }
        }
        // Traigo el que necesito de memoria
        block_t *block = memory_get_block(cache->pri_mem, get_block_num(address));
        way = get_way_last_block(cache->sets[set]);
        set_insert_block(cache->sets[set], block, way, tag);
    } else {
        cache->hits++;
    }
    return read_block(cache->sets[set], tag, offset);
}


void write_byte(unsigned int address, unsigned char value) {
    address &= 0xFFFF;
    uint16_t tag = get_tag(address);
    unsigned int set = find_set(address);
    unsigned int offset = get_offset(address);
    unsigned int way = search_block_position(cache->sets[set], tag);
    if (way == -1) {
        cache->miss++;
        if (set_is_full(cache->sets[set])) {
            block_t *last = set_get_last_block(cache->sets[set]);
            if (last != NULL) {
                // Me fijo si el que tengo reemplazar, esta dirty o no, si esta lo escribo a memory
                if (block_is_dirty(last)) {
                    memory_write_block(cache->pri_mem,last,address,tag);
                }
            }
        }        
        // Traigo el que necesito de memoria
        way = get_way_last_block(cache->sets[set]);
        block_t *block = memory_get_block(cache->pri_mem, get_block_num(address));
        block_write_byte(block,value,offset);
        set_insert_block(cache->sets[set], block, way, tag);
    } else {
        // Escribo y nada mas, cambio el dirty a 1
        cache->hits++;
        set_write_block(cache->sets[set], way, offset, value);
    }
}

float get_miss_rate() {
    return (cache->miss / (float)(cache->miss + cache->hits));
}