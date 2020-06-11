#ifndef BLOCK_H
#define BLOCK_H
#define BLOCK_SIZE 64
#include <stdint.h>
#include <stdbool.h>

typedef struct block {
  char *bytes;
  uint8_t valid;
  uint8_t dirty;
  uint8_t tag; 
} block_t;

/**
 * Constructor 
*/
void block_create(block_t *self);

/**
 * Funcion encargada de copiar un block a otro
*/
void block_copy(block_t *self, block_t *other, uint8_t tag);

/**
 * Destructor
*/
void block_destroy(block_t *self);

/**
 * Funcion que evalua si dos bloque son iguales
*/
bool blocks_are_equal(block_t *self, block_t *other);

/**
 * Funcion que devuelve el byte ubicado en la posicion recibida
*/
unsigned char block_read_byte(block_t *self, uint8_t position);

/**
 * Funcion que escribe un byte en la posicion recibida 
*/
void block_write_byte(block_t *self, char byte, uint8_t position);

/**
 * Funcion que resetea el bloque, setea todos los valores en 0
*/
void block_reset(block_t *self);

/**
 * Getter del tag
*/
uint8_t block_get_tag(block_t *self);

/**
 * Devuelve si el boque es valido
*/
uint8_t block_is_valid(block_t *self);

/**
 * Devuelve si el boque fue modificado o no
*/
uint8_t block_is_dirty(block_t *self);

/**
 * Settea el byte de valid en 1
*/
void block_validate(block_t *self);

#endif