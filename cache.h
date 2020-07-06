#ifndef CACHE_H
#define CACHE_H
#define NUMBER_SETS 128
#define NUMBER_WAYS 4
#include "memory.h"
#include "set.h"
#include <stdint.h>

typedef struct cache {
  memory_t *pri_mem;
  set_t **sets;
  size_t hits;
  size_t miss;
} cache_t;

cache_t *cache;

void create_cache(memory_t *mp);

/**
 * Inicializa la memoria principal simulada en 0, los bloques de caché como
 * inválidos y la tasa de misses a 0.
 * 
 * Llamar a este método nuevamente provocará que se reinicie la memoria al 
 * estado inicial (memoria en 0, bloques inválidos y tasa de misses en 0).
 */
void init();

/**
 * Devuelve el offset del byte del bloque de memoria al que mapea la dirección
 * address.
 */
unsigned int get_offset(unsigned int address);

/**
 * Devuelve el el via de caché al que mapea la dirección address.
 */
unsigned int find_set(unsigned int address);

/**
 * Devuelve la vı́a en la que está el bloque más "viejo" dentro de un conjunto, 
 * utilizando el campo correspondiente de los metadatos de los bloques del 
 * conjunto.
 */
unsigned int select_oldest(unsigned int setnum);

/**
 * devolver la vı́a en la que se encuentra almacenado el bloque co-
 * rrespondiente a tag en el conjunto index, o -1 si ninguno de los tags
 * coincide.
 */
int compare_tag(unsigned int tag, unsigned int set);

/**
 * Lee el bloque blocknum de memoria y lo guarda en el conjunto y vı́a indicados 
 * en la memoria caché.
 */
void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set);

/**
 * Busca el valor del byte correspondiente a la posición address en la caché; 
 * si éste no se encuentra en la caché carga ese bloque. 
 * 
 * Devuelve el valor del byte almacenado en la dirección indicada.
 */
unsigned char read_byte(unsigned int address);

/**
 * Escribe el valor value en la posición address de la memoria, y en la posición
 * correcta del bloque que corresponde a address, si el bloque se encuentra en 
 * la caché. 
 * Si no se encuentra, escribe el valor solamente en la memoria.
 */
void write_byte(unsigned int address, unsigned char value);

/**
 * Devuelve el porcentaje de misses desde que se inicializó la caché.
 */
float get_miss_rate();

/**
 * Destructor
*/
void destroy_cache();

#endif // _CACHE_H_
