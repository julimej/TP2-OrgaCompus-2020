#include "cola.h"
#include <stdlib.h>
#include <stdio.h>

struct nodo {
	void *dato;
	struct nodo *prox;
};

typedef struct nodo nodo_t;

struct cola {
	nodo_t *ultimo;
	nodo_t *primero;
};

/* ******************************************************************
 *                    PRIMITIVAS DE LA COLA                         *
 * *****************************************************************/

nodo_t *_nodo_crear(void *dato) {
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL) {
		free(nodo);
		return NULL;
	}
	nodo->dato = dato;
	return nodo;
}

cola_t *cola_crear(void) {
	cola_t *cola = malloc(sizeof(cola_t));
	if (cola == NULL) {
		free(cola);
		return NULL;
	}
	cola->primero = NULL;
	return cola;
}

void cola_destruir(cola_t *cola, void destruir_dato(void *)) {
	nodo_t *act = cola->primero;
	while (act != NULL) {
		nodo_t *aux = act->prox;
		if (destruir_dato != NULL) {
			destruir_dato(act->dato);
		}
		free(act);
		act = aux;
	}
	free(cola);
}

bool cola_esta_vacia(const cola_t *cola) {
	return cola->primero == NULL;
}

bool cola_encolar(cola_t *cola, void *valor) {
	nodo_t *nodo = _nodo_crear(valor);
	if (nodo == NULL) {
		return false;
	}

	if (cola->primero == NULL) {
		cola->primero = nodo;
		cola->ultimo = nodo;
		cola->ultimo->prox = NULL;
	} else {
		cola->ultimo->prox = nodo;
		cola->ultimo = nodo;
		cola->ultimo->prox = NULL;
	}
	return true;
}

void *cola_ver_primero(const cola_t *cola) {
	return cola_esta_vacia(cola) ? NULL : cola->primero->dato;
}

void *cola_desencolar(cola_t *cola) {
	if (cola_esta_vacia(cola))
		return NULL;
	void *aux = cola->primero->dato;
	nodo_t *nodoaux = cola->primero;
	cola->primero = cola->primero->prox;
	free(nodoaux);
	return aux;
}
