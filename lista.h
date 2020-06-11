#ifndef LISTA_H
#define LISTA_H

#include <stdlib.h>
#include <stdio.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct nodo {
	void* dato;
	struct nodo* prox;
} nodo_t;

struct lista {
	nodo_t* prim;
	nodo_t* ultimo;
	size_t largo;
};

struct lista_iter  {
	nodo_t* n_act;
	nodo_t* n_ant;
	lista_t* lista;
};


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea un nodo
// Post: el nodo fue creado.
nodo_t* _nodo_crear(void* dato) {
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL) {
		free(nodo);
		return NULL;
	}
	nodo->dato = dato;
	return nodo;
}


// Crea una lista.
// Post: la lista fue creada.
lista_t *lista_crear(void) {
	lista_t* lista = malloc(sizeof(lista_t));
	if (lista == NULL) {
		free(lista);
		return NULL;
	}

	lista->prim = NULL;
	lista->ultimo = NULL;
	lista->largo = 0;
	return lista;
}

// Devuelve true si la lista esta vacia, de caso contrario devuelve false.
// Pre: lista fue creada.
// Post: se delvio true si la lista esta vacia, o false de caso contrario.
bool lista_esta_vacia(const lista_t *lista) {
	return lista->prim == NULL;
}

// Inserta un elemento al principio de la lista. Devuelve true si se inserto,
// del caso contrario, devuelve false.
// Pre: la lista fue creada.
// Post: el elemento fue insertado al principio y se devolvio true o del caso contrario no 
// fue insertado y devolvio false.
bool lista_insertar_primero(lista_t *lista, void *dato) {
	nodo_t* nodo = _nodo_crear(dato);
	if (nodo == NULL) return false;

	if (lista_esta_vacia(lista)) {
		lista->prim = nodo;
		lista->prim->prox = NULL;
		lista->ultimo = nodo;
		lista->ultimo->prox = NULL;
	}
	else {
		nodo_t* aux = lista->prim;
		lista->prim = nodo;
		lista->prim->prox = aux;
	}
	lista->largo ++;
	return true;
}

// Inserta un elemento al final de la lista. Devuelve true si se inserto,
// del caso contrario, devuelve false.
// Pre: la lista fue creada.
// Post: el elemento fue insertado al final y se devolvio true o del caso contrario no 
// fue insertado y devolvio false.
bool lista_insertar_ultimo(lista_t *lista, void *dato) {
	nodo_t* nodo = _nodo_crear(dato);
	if (nodo == NULL) return false;

	if (lista_esta_vacia(lista)) {
		lista->prim = nodo;
		lista->prim->prox = NULL;
		lista->ultimo = nodo;
		lista->ultimo->prox = NULL;
	}
	else {
		nodo_t* aux = lista->ultimo;
		lista->ultimo = nodo;
		lista->ultimo->prox = NULL;
		aux->prox = lista->ultimo;
	}

	lista->largo ++;
	return true;
}

// Borra el primer elemento de la lista y lo devuelve. Pero si esta vacia devuleve NULL.
// Pre: la lista fue creada.
// Post: El elemento fue borrado y devuelto o, de caso contario, si esta vacia devuelve NULL.
void *lista_borrar_primero(lista_t *lista) {
	if (lista_esta_vacia(lista)) return NULL;
	void* dato_aux = lista->prim->dato;
	nodo_t* nodo_aux = lista->prim;
	lista->prim = lista->prim->prox;
	lista->largo --,
	free(nodo_aux);
	if (lista_esta_vacia(lista)) lista->ultimo = NULL;
	return dato_aux;
}


// Devuelve el primer elemento de la lista. Si esta vacia devuelve null.
// Pre: la lista fue creada.
// Post: Se devolvio el elemento primer elemento de la lista o, si esta vacia se devolvio NULL.
void *lista_ver_primero(const lista_t *lista) {
	return lista->prim == NULL ? NULL:lista->prim->dato; 
}


// Devuelve el primer elemento de la lista. Si esta vacia devuelve null.
// Pre: la lista fue creada.
// Post: Se devolvio el ultimo elemento de la lista o, si esta vacia se devolvio NULL.
void *lista_ver_ultimo(const lista_t* lista) {
	return lista->ultimo == NULL ? NULL:lista->ultimo->dato;
}

// Devuelve el largo de la lista.
// Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista) {
	return lista->largo;
}

// Destruye la lista.
// Pre: la lista fue creada.
// Post: la lista fue destruida.
void lista_destruir(lista_t *lista, void destruir_dato(void *)) {
	nodo_t* act = lista->prim;
	while (act != NULL) {
		nodo_t* aux = act->prox;
		if (destruir_dato != NULL) {
			destruir_dato(act->dato);
		}
		free(act);
		act = aux;
		}
	free(lista);
}




/* ******************************************************************
 *                    PRIMITIVAS DE LA ITERADOR INTERNO
 * *****************************************************************/


void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
	nodo_t* act = lista->prim;
	while (act) {
		if (!visitar(act->dato, extra)) {
			return;
		}
		act = act->prox;
	}
	return;
}

/* ******************************************************************
 *                    PRIMITIVAS DE LA ITERADOR EXTERNO
 * *****************************************************************/

// Crea el iterador.
// Post: el iterador fue creado.
lista_iter_t *lista_iter_crear(lista_t *lista) {
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (iter == NULL) {
		free(iter);
		return NULL;
	}

	iter->lista = lista;
	iter->n_act = iter->lista->prim;
	iter->n_ant = NULL;
	return iter;
}

// Avanza una posicion en la lista el iteradir. Devuelve true de haber podido, 
// en caso contrario, devuelve false.
// Pre: la lista fue creada.
// Post: Avanzo el iterador y se devolvio true o se quedo en la misma posicio y devolvio false. 
bool lista_iter_avanzar(lista_iter_t *iter) {
	if (lista_esta_vacia(iter->lista)) return false;
	if (lista_iter_al_final(iter)) return false;
	iter->n_ant = iter->n_act;
	iter->n_act = iter->n_act->prox;
	return true;
}
	
// Devuelve el elemento al cual apunta actualmente
// Pre: la lista fue creada.
// Post: se devolvio el dato del nodo actual
void *lista_iter_ver_actual(const lista_iter_t *iter) {
	return iter->n_act == NULL ? NULL:iter->n_act->dato;
}

// Devulve true si esta al final de la lista, o false de ser lo contrario.
// Pre: la lista fue creada.
// Post: Devolvio true si esta al final o false, en caso de no estarlo.
bool lista_iter_al_final(const lista_iter_t *iter) {
	return iter->n_act == NULL;
}

// Destruye el iterador
// Pre: el iterador fue creado
// Post: el iterado fue destruido
void lista_iter_destruir(lista_iter_t *iter) {
	free(iter);
} 

// Inserta un elmento en la posicion actual de la lista. Devuelve True en caso afirmativo,
// o devuelve false, en caso contario.
// Pre: la lista fue creada.
// Post: El elemento fue insertado y e devolvio true, o en caso contrario, no se inserto y se devolvio false.
bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
	if (iter->n_act == iter->lista->prim) {
		if(lista_insertar_primero(iter->lista, dato)) { 
			iter->n_act = iter->lista->prim;
			return true;
		}
		return false;
	}
	else if (lista_iter_al_final(iter)) {
		if (lista_insertar_ultimo(iter->lista, dato)) {
			iter->n_ant = iter->n_act;
			iter->n_act = iter->lista->ultimo;
			return true;
		}
		return false;
	}
	nodo_t* nodo = _nodo_crear(dato);
	nodo_t* aux = iter->n_act;
	iter->n_act = nodo;
	iter->n_ant->prox = iter->n_act;
	iter->n_act->prox = aux;
	iter->lista->largo ++;
	return true;
}



void *lista_iter_borrar(lista_iter_t *iter) {
	if (iter->n_act == NULL) return NULL;
	nodo_t* aux_nodo = iter->n_act;
	void* aux_dato = aux_nodo->dato;
	if (iter->n_act == iter->lista->prim) {
		lista_borrar_primero(iter->lista);
		iter->n_act = iter->lista->prim;
	}
	else {
		iter->n_act = aux_nodo->prox;
		iter->n_ant->prox = iter->n_act;
		iter->lista->largo --;
		free(aux_nodo);
	}
	if (iter->n_act == NULL) iter->lista->ultimo = iter->n_ant;
	return aux_dato;
}

#endif // LISTA_H