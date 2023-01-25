#ifndef _DVECTOR_H_
#define _DVECTOR_H_

/** \mainpage Dynamic Vector
*
* @brief This library implements a vector with dynamic size.
*
*/
/**
* @brief Data type of the vector elements.
 */
 typedef int vdata_t;

/* The Dynamic Vector data type.
 */
 typedef struct vector vector_t;

 /**
 * @brief Allocates an empty vector.
 * @return empty vector.
 */
 vector_t* v_create();

/**
* @brief Frees a vector and the memory it uses.
* @param v The vector to be freed.
*/
void v_free(vector_t *v);

/**
* @brief prints a vector.
* @param v The vector to be printed.
*/
void v_print(const vector_t *v);

/**
* @brief  if a vector is empty.
* @param v The vector.
* @return 1 if empty, 0 otherwise.
*/
int v_is_empty(const vector_t *v);

/**
* @brief Returns the size of a given vector.
* @param v The vector.
* @return The number of elements stored in the vector.
*/
size_t v_size(const vector_t *v);

/**
* @brief Returns element at a given index.
* Program exits if index is out of bounds.
* @param v The vector.
* @param index The desired index.
* @return the value stored at the corresponding index.
*/
vdata_t v_get(const vector_t *v, size_t index);

/**
* @brief Inserts a new value after the last element of the vector.
* @param v The vector.
* @param value The value to be inserted.
*/
void v_push_back(vector_t *v, vdata_t value);

/**
* @brief Removes the first element in the vector and returns its value.
* Program exists if vector is empty.
* @param v The vector.
* @return The value previously stored in the first position.
*/
void v_pop_front(vector_t *v);


#endif