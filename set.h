#ifndef SET_H
#define SET_H
#define NUMBER_BLOCKS_PER_SET 4
#define NUMBER_BLOCK_INVALIDA (NUMBER_BLOCKS_PER_SET + 1)
#include <stdint.h>
#include "block.h"
#include "cola.h"

struct set {
  block_t **blocks;
  int blocks_inserted;
};

typedef struct set set_t;

/**
 * Constructor
*/
void set_create(set_t *set);

/**
 * Destructor
*/
void set_destroy(set_t *set);

/**
 * Inserta un bloque en el way indicado con el tag indicado
*/
void set_insert_block(set_t *set, block_t *block, int way, unsigned int tag);

/**
 * Resetea todo el set, es decir que la memoria queda vacia
*/
void set_reset(set_t *set);

/**
 * Devuelve la posicion del bloque que coincida con ese tag
*/
int set_get_block_position(set_t *set, uint8_t tag);

/**
 * Lee de un blque con el tag indicado, en la posicion que indica el offset
*/
unsigned char set_read_block(set_t *set,unsigned int way, unsigned int offset);

/**
 * Escribe en un blque con el tag indicado, en la posicion que indica el offset, el valor recibido
*/
void set_write_block(set_t *set, unsigned int way, unsigned int offset, char value);

/**
 * Selecciona el bloque que menos se uso en las ultimas iteraciones
*/
unsigned int set_select_oldest(set_t *set);

/**
 * Devuelve si el set esta lleno
*/
bool set_is_full(set_t* set);

/**
 * Devuelve el way donde debe ser insertado el bloque nuevo
*/
int set_get_way_to_insert(set_t* set);

/**
 * Realiza un update de los bloque con respecto a si fue usado o no, actualizando el contardor
*/
void set_update_counters(set_t* set, unsigned int last_used);

#endif