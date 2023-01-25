#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dvector.h"

/**
*@brief The Dynamic Vector data type
*/
 struct vector {
 /** The number of elements in the vector */
 size_t size;
 /** The amount of allocated memory */
 size_t capacity;
 /** The allocated memory */
 vdata_t *data;
 };

 /**
 * The minimum allocated memory upon creation.
 */
 const size_t V_MIN_CAPACITY = 16;

 /**
 * Extends allocated memory if entirely used by the stored elements.
 * @param v The vector.
 */
void _v_check_extend(vector_t *v){
    if (v->size==v->capacity) {
        vdata_t* new_data = (vdata_t*) realloc( v->data, v->capacity*2*sizeof(vdata_t) ); /* see https://en.cppreference.com/w/c/memory/realloc
*/
        if (!new_data) {
            v_free(v); /* This is a reminder that in case of error,* the programmer is in charge of the original memory */
            printf ("Error: Failed memory allocation\n");
            exit(EXIT_FAILURE);
        }
    v->data = new_data;
    v->capacity *= 2;
    }
}

/**
* Shrinks allocated memory if larger than twice the number of stored elements.
* @param v The vector.
*/
void _v_check_shrink(vector_t *v){
    size_t half_capacity = v->capacity/2;
    if( half_capacity >= V_MIN_CAPACITY &&v->size <= half_capacity ) {
        vdata_t* new_data = (vdata_t*) realloc( v->data, half_capacity *sizeof(vdata_t) ); /* see https://en.cppreference.com/w/c/memory/realloc
*/
    if (!new_data) {
        v_free(v); /* This is a reminder that in case of error,* the programmer is in charge of the original memory */
        printf ("Error: Failed memory allocation\n");
        exit(EXIT_FAILURE);
    }
    v->data = new_data;
    v->capacity = half_capacity;
    }
}

/**
* Fatal exit if the given index is beyond vector size.
* @param v The vector.
* @param index The index.
*/
void _v_check_index(const vector_t* v, size_t index) {
    if( index >= v->size ){
        printf("Error: Index %lu out of bounds for vector of size %lu\n", index, v->size);
        exit(EXIT_FAILURE);
    }
}

/**
* Fatal exit if the given vector is empty.
* @param v The vector.
*/
void _v_check_non_empty(const vector_t* v) {
    if( v_is_empty(v) ){
        printf("Error: Empty vector\n");
        exit(EXIT_FAILURE);
    }
}
/**
 * @brief crea un vector_t
 * 
 * @return vector_t* 
 */
vector_t* v_create() {
    vector_t* v = (vector_t*) malloc ( sizeof(vector_t) );
    v->size = 0;
    v->capacity = V_MIN_CAPACITY; /* initial capacity */
    v->data = (int*) malloc( v->capacity * sizeof(vdata_t) );
    if (!v->data) {
        printf ("Error in memory allocation\n");
        exit(EXIT_FAILURE);
    }
    return v;
}
/**
 * @brief effettua la free del vector
 * 
 * @param v 
 */
void v_free(vector_t *v){
    free(v->data);
    free(v);
}

/**
 * @brief Restituisce un elemento a un dato indice.
 *
 *
 * 
 * @param v 
 * @param index 
 * @return int 
 */
int v_get(const vector_t *v, size_t index){
    _v_check_index(v, index);
    return v->data[index];
}

/**
 * @brief stampa il vector
 * 
 * @param v 
 */
void v_print(const vector_t *v) {
    size_t i;
    printf("[ ");
    for (i=0; i<v->size; i++)
        printf ("%d ", (int) v->data[i]);
        printf("]\n");
}

/**
 * @brief mi dice se il vector è vuoto
 * 
 * @param v 
 * @return int 
 */
int v_is_empty(const vector_t *v){
    return v->size == 0;
}

/**
 * @brief mi dice la size del vector
 * 
 * @param v 
 * @return size_t 
 */
size_t v_size(const vector_t *v){
    return v->size;
}

/**
 * @brief aggiunge l'elemento in fondo al vettore
 * 
 * @param v 
 * @param value 
 */
void v_push_back(vector_t *v, vdata_t value){
    _v_check_extend(v);
    v->data[v->size++] = value;
}


/**
 * @brief elimina l'elemento in prima posizione
 * 
 * @param v 
 */
void v_pop_front(vector_t *v){
    size_t i;
    _v_check_non_empty(v);
    for (i=0; i<v->size; i++)
        v->data[i] = v->data[i+1];
    v->size--;
    _v_check_shrink(v);
 }

