#include "lista.h"
#include "cola.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


nodo_t *nodo_crear(void *elemento)
{
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (!nodo)
		return NULL;
	nodo->elemento = elemento;
	nodo->siguiente = NULL;

	return nodo;
}

void nodo_destruir(nodo_t *nodo)
{
	free(nodo);
}

lista_t *lista_crear()
{
	return calloc(1,sizeof(lista_t));
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (!lista)
		return NULL;

	nodo_t *nodo = nodo_crear(elemento);
	if (!nodo)
		return NULL;

        if (lista_vacia(lista)){
		lista->nodo_inicio = nodo;
		lista->nodo_fin = nodo;
		lista->cantidad ++;

		return lista;
	}
	
	lista->nodo_fin->siguiente = nodo;        
	lista->nodo_fin = nodo;
	lista->cantidad++;

	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (!lista)
		return NULL;

	size_t i = 0;

	if (lista_vacia(lista) || posicion >= lista_tamanio(lista)) {
		lista = lista_insertar(lista, elemento);
			return lista;
	}

	nodo_t *nodo = nodo_crear(elemento);
	if (!nodo)
		return NULL;

	if (posicion == 0) {
		nodo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nodo;
		lista->cantidad ++;

		return lista;
	}
	
	nodo_t *aux = lista->nodo_inicio;
	while (i < posicion -1) {
		aux = aux->siguiente;
		i++;
	}
	
	nodo->siguiente = aux->siguiente;
	aux->siguiente = nodo;
	lista->cantidad ++;

	return lista;
}

void *lista_quitar(lista_t *lista) 
{
	if (!lista || lista_vacia(lista))
		return NULL;

	void *elemento = NULL;
	nodo_t *nodo_a_eliminar = NULL;
	nodo_t *aux = lista->nodo_inicio;

	if (lista_tamanio(lista) == 1) {
	
		elemento = lista->nodo_inicio->elemento;
		nodo_destruir(lista->nodo_inicio);
	
		lista->nodo_inicio = NULL;
		lista->nodo_fin = NULL;
		lista->cantidad = 0;
		
		return elemento;
	}

	while (aux->siguiente->siguiente != NULL)
		aux = aux->siguiente;
	
	elemento = aux->siguiente->elemento;
	nodo_a_eliminar = aux->siguiente;
	nodo_destruir(nodo_a_eliminar); 
	
	aux->siguiente = NULL;
	lista->nodo_fin = aux;
	lista->cantidad --;

	return elemento;

}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || lista_vacia(lista))
		return NULL;
	
	size_t i = 0;
	void *elemento = NULL;
	nodo_t *aux = NULL;
	nodo_t *nodo_a_eliminar = NULL;

	if (lista_tamanio(lista) == 1 || posicion >= lista_tamanio(lista)) { 
		return lista_quitar(lista);
	}
	
	if (posicion == 0) {
		elemento = lista->nodo_inicio->elemento;
		nodo_a_eliminar = lista->nodo_inicio;
		lista->nodo_inicio = lista->nodo_inicio->siguiente;
	
		nodo_destruir(nodo_a_eliminar);
		lista->cantidad --;

		return elemento;
	}
	
	aux = lista->nodo_inicio ;
	while (i < posicion -1) {
		aux = aux->siguiente;
		i++;
	}
	
	elemento = aux->siguiente->elemento;
	nodo_a_eliminar = aux->siguiente;
	aux->siguiente = aux->siguiente->siguiente;
	
	nodo_destruir(nodo_a_eliminar);
	lista->cantidad --;

	return elemento;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || lista_vacia(lista) || posicion >= lista_tamanio(lista))
		return NULL;

	if (posicion == 0 || lista_tamanio(lista) == 1)
		return lista_primero(lista);

	if (posicion == lista_tamanio(lista)-1)
		return lista_ultimo(lista);
	
	int i = 0;
	nodo_t *aux = lista->nodo_inicio;

	while (i < posicion ) {
		aux = aux->siguiente;
		i++;
	}
	
	return aux->elemento;	
}


void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (!lista || lista_vacia(lista)) 
		return NULL;

	nodo_t *aux = lista->nodo_inicio;

	while (aux) {
		if (comparador){
			if (!comparador(aux->elemento,contexto))
				return aux->elemento;
		}
		aux = aux->siguiente;
	}
	return NULL;
}

void *lista_primero(lista_t *lista)
{
	return (!lista || lista_vacia(lista)) ? NULL : lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	return (!lista || lista_vacia(lista)) ? NULL : lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t *lista)
{
	return (!lista || lista->cantidad == 0) ? true : false;
}

size_t lista_tamanio(lista_t *lista)
{
	return ((!lista || lista_vacia(lista))) ? 0 : lista->cantidad;
}

void lista_destruir(lista_t *lista)
{
	if (!lista) 
		return;
	
	while (lista->nodo_inicio) {
		lista_quitar_de_posicion(lista, 0);
	}
	if (lista->nodo_inicio)
		free(lista->nodo_inicio);
	free(lista);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (!lista) 
		return;

	while (lista->nodo_inicio) {
		if (funcion) 
			funcion(lista_primero(lista));
		lista_quitar_de_posicion(lista, 0);
	}
	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista) 
		return NULL;

	lista_iterador_t *iterador = calloc(1,sizeof(lista_iterador_t));
	if (!iterador)
		return NULL;

	iterador->lista = lista;
	iterador->corriente = lista->nodo_inicio;

	if (lista_vacia(lista)) 
		return iterador;

	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{	
	if (!iterador || lista_vacia(iterador->lista))
		return false;

	return iterador->corriente;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!iterador || lista_vacia(iterador->lista) || !iterador->corriente)
		return false;

	iterador->corriente = iterador->corriente->siguiente;

	return iterador->corriente;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || lista_vacia(iterador->lista) || !iterador->corriente)
		return NULL;

	return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (!lista || !funcion || lista_vacia(lista))
		return 0;

	size_t i = 0;
	lista_iterador_t *iterador = NULL;
	iterador = lista_iterador_crear(lista);
	
	while (lista_iterador_tiene_siguiente(iterador)) {
		
		if (funcion(lista_iterador_elemento_actual(iterador),contexto)) {
			i++;						
			lista_iterador_avanzar(iterador);
		}
		else {
			i++;
			break;
		}	
	}
	lista_iterador_destruir(iterador);

	return i;
}
