#include <stdlib.h>
#include "memory.h"
#define NUMBER_OF_BLOCKS 1024
#define MEMORY_SIZE (1024*64)

void memory_create(memory_t *memory) {
    memory->memory = calloc(MEMORY_SIZE,sizeof(char));
    for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
        memory->memory[i] = calloc(1, sizeof(block_t));
        block_create(memory->memory[i]);
    }
}

void memory_destroy(memory_t *memory) {
    for (size_t i = 0; i < NUMBER_OF_BLOCKS; i++) {
        block_destroy(memory->memory[i]);
        free(memory->memory[i]);
    }
    free(memory->memory);
}


uint16_t get_block_number(unsigned int address) {
    return (address >> 7);
}

uint16_t get_block_offset(unsigned int address) {
    uint16_t offset = (address << 9);
    return offset >> 9;
}

void memory_write_byte(memory_t *memory, unsigned int address, unsigned char byte) {
    uint16_t block_num = get_block_number(address);
    uint16_t offset = get_block_offset(address);
    block_write_byte(memory->memory[block_num], offset, byte);
}

unsigned char memory_read_byte(memory_t *memory, unsigned int num_block, unsigned int offset) {
    return block_read_byte(memory->memory[num_block], offset);
}

block_t *memory_get_block(memory_t *memory, unsigned int num_block) {
    return memory->memory[num_block];
}

void memory_write_block(memory_t *memory, block_t *block, unsigned int address, uint8_t tag) {
    uint16_t block_num = get_block_number(address);
    block_copy(block, memory->memory[block_num], tag);
}