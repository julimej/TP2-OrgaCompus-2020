#ifndef BLOCK_H
#define BLOCK_H
#define BLOCK_SIZE 128
#include <stdint.h>
#include <stdbool.h>

typedef struct block {
  char *bytes;
  uint8_t valid;
  uint8_t tag; 
  uint8_t lastused;
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
void block_write_byte(block_t *self, uint8_t position, char byte);

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
bool block_is_valid(block_t *self);

/**
 * Agrega uno a last_used
 **/
void block_add_not_used(block_t *self);

/**
 * Settea el lastused en cero
*/
void block_set_used(block_t *self);

/**
 * Settea el byte de valid en 1
*/
void block_validate(block_t *self);

#endif