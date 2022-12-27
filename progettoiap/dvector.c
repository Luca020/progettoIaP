#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dvector.h"

/**
* The Dynamic Vector data type
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

vector_t* v_clone(const vector_t *v) {
    vector_t* c = (vector_t*) malloc ( sizeof(vector_t) );
    c->size = v->size;
    c->capacity = v->capacity;
    c->data = (int*) malloc( c->capacity * sizeof(vdata_t) );
    if (!c->data) {
        printf ("Error in memory allocation\n");
        exit(EXIT_FAILURE);
    }
    memcpy(c->data, v->data, v->size * sizeof(vdata_t) );
    return c;
}

void v_free(vector_t *v){
    free(v->data);
    free(v);
}

void v_print(const vector_t *v) {
    size_t i;
    printf("[ ");
    for (i=0; i<v->size; i++)
        printf ("%d ", (int) v->data[i]);
        printf("]\n");
}

int v_is_empty(const vector_t *v){
    return v->size == 0;
}

size_t v_size(const vector_t *v){
    return v->size;
}

int v_get(const vector_t *v, size_t index){
    _v_check_index(v, index);
    return v->data[index];
}

void v_set(vector_t *v, size_t index, vdata_t value){
    _v_check_index(v, index);
    v->data[index] = value;
}

void v_push_back(vector_t *v, vdata_t value){
    _v_check_extend(v);
    v->data[v->size++] = value;
}

void v_pop_back(vector_t *v){
    
    _v_check_non_empty(v);
    v->size--;
    _v_check_shrink(v);
 }

void v_push_front(vector_t *v, vdata_t value){
    size_t i;
    _v_check_extend(v);
    for (i=v->size; i>0; i--)
        v->data[i] = v->data[i-1];
        v->size++;
        v->data[0] = value;
}

void v_pop_front(vector_t *v){
    size_t i;
    _v_check_non_empty(v);
    for (i=0; i<v->size; i++)
        v->data[i] = v->data[i+1];
    v->size--;
    _v_check_shrink(v);
 }

void v_insert_at(vector_t *v, vdata_t value, size_t index){
    size_t i;
    _v_check_index(v, index);
    _v_check_extend(v);
    for (i=v->size; i>index; i--)
        v->data[i] = v->data[i-1];
    v->size++;
    v->data[index] = value;
}

void v_delete_at(vector_t *v, size_t index){
    size_t i;
    _v_check_index(v, index);
    for (i=index; i<v->size; i++)
        v->data[i] = v->data[i+1];
    v->size--;
    _v_check_shrink(v);
}
