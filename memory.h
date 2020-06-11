#ifndef MEMORY_H
#define MEMORY_H
#include <stdio.h>
#include "block.h"

struct memory {
  block_t **memory;
};

typedef struct memory memory_t;

/** 
 * Constructor
 */
void memory_create(memory_t *memory);

/**
 * Destructor
*/
void memory_destroy(memory_t *memory);

/**
 * Funcion que escribe un byte, que es recibido por memoria, en una addres recibido
*/
void memory_write_byte(memory_t *memory, unsigned int address, unsigned char byte);

/**
 * Funcion que lee un byte en una posicion recibida dentro de un bloque especifico
*/
unsigned char memory_read_byte(memory_t *memory, unsigned int num_block, unsigned int offset);

/**
 * Funcion que devuelve el bloque con el numero de bloque especific recibido
*/
block_t *memory_get_block(memory_t *memory, unsigned int num_block);

/**
 * Funcion que obtiene el numero de bloque de una direccion
*/
uint16_t get_block_number(unsigned int address);

/**
 * Funcion que devuelve la offset dentro del bloque
*/
uint16_t get_block_offset(unsigned int address);

void memory_write_block(memory_t *memory, block_t *block, unsigned int address, uint8_t tag);

#endif
