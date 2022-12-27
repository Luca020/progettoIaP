#ifndef _DVECTOR_H_
#define _DVECTOR_H_

/** \mainpage Dynamic Vector
*
* This library implements a vector with dynamic size.
*
*/
/**
* Data type of the vector elements.
 */
 typedef int vdata_t;

/* The Dynamic Vector data type.
 */
 typedef struct vector vector_t;

 /**
 * Allocates an empty vector.
 * @return empty vector.
 */
 vector_t* v_create();

 /**
 * Clones a vector (deep copy).
 * @return vector.
*/
 vector_t* v_clone(const vector_t *v);

/**
* Frees a vector and the memory it uses.
* @param v The vector to be freed.
*/
void v_free(vector_t *v);

/**
* Prints a vector.
* @param v The vector to be printed.
*/
void v_print(const vector_t *v);

/**
* Checks if a vector is empty.
* @param v The vector.
* @return 1 if empty, 0 otherwise.
*/
int v_is_empty(const vector_t *v);

/**
* Returns the size of a given vector.
* @param v The vector.
* @return The number of elements stored in the vector.
*/
size_t v_size(const vector_t *v);

/**
* Returns element at a given index.
* Program exits if index is out of bounds.
* @param v The vector.
* @param index The desired index.
* @return the value stored at the corresponding index.
*/
vdata_t v_get(const vector_t *v, size_t index);
/**
* Stores a value at a given index.
* Program exits if index is out of bounds.
* @param v The vector.
* @param index The desired index.
* @param value The value to be stored.
*/
void v_set(vector_t *v, size_t index, vdata_t value);

/**
* Inserts a new value after the last element of the vector.
* @param v The vector.
* @param value The value to be inserted.
*/
void v_push_back(vector_t *v, vdata_t value);

/**
* Removes the last element in the vector and returns its value.
* Program exists if vector is empty.
* @param v The vector.
* @return The value previously stored in the last position.
*/
void v_pop_back(vector_t *v);

/**
* Inserts a new value before the first element of the vector.
* @param v The vector.
* @param value The value to be inserted.
*/
void v_push_front(vector_t *v, vdata_t value);

/**
* Removes the first element in the vector and returns its value.
* Program exists if vector is empty.
* @param v The vector.
* @return The value previously stored in the first position.
*/
void v_pop_front(vector_t *v);

/**
* Inserts a new value at given position by shifting existing elements to the right.
* Program exits if index is out of bounds.
* @param v The vector.
* @param value The value to be inserted.
* @param index The position where to insert the value.
*/
void v_insert_at(vector_t *v, vdata_t value, size_t index);

/**
* Deletes element at given position by shifting existing elements to the left.
* Program exits if index is out of bounds.
*
 @param v The vector.
* @param index The position to be removed.
*/
void v_delete_at(vector_t *v, size_t index);

#endif