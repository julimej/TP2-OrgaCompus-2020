#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "block.h"
#define BLOCK_SIZE 64

void block_create(block_t *self) {
    self->bytes = calloc(BLOCK_SIZE, sizeof(char));
    self->valid = 0;
    self->lastused = 0;
}

void block_copy(block_t *self, block_t *other, uint8_t tag) {
    other->tag = tag;
    memcpy(other->bytes, self->bytes, BLOCK_SIZE);
    self->valid = 1;
    self->lastused = 0;
}

void block_destroy(block_t *self) {
    if (self == NULL){
        return;
    }
    free(self->bytes);
}

bool blocks_are_equal(block_t *self, block_t *other) {
    if (!(block_is_valid(self)) || !(block_is_valid(other)))
        return false;
    return self->tag == other->tag;
}

unsigned char block_read_byte(block_t *self, uint8_t position) {
    return self->bytes[position];
}

void block_write_byte(block_t *self, char byte, uint8_t position) {
    self->bytes[position] = byte;
}

void block_reset(block_t *self) {
    memset(self->bytes, '\0', BLOCK_SIZE);
    self->valid = 1;
    self->lastused = 0;
}

uint8_t block_get_tag(block_t *self) {
    return self->tag;
}

uint8_t block_is_valid(block_t *self) {
    return self->valid == 1;
}

void block_validate(block_t *self) {
    self->valid = 1;
}

void block_add_not_used(block_t *self){
    self->lastused ++;
}

void block_set_used(block_t *self) {
    self->lastused = 0;
}
