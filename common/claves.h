#ifndef CLAVES_H
#define CLAVES_H

// Max size of string value
#define MAX_STR_SIZE 256
// Max size of vector value
#define MAX_VEC_SIZE 32

/**
 * Initializes the data structure
 *
 * @return Error code
 */
int init();

/**
 * Deletes an item from the data structure
 *
 * @param key Key of the item to delete
 * @return Error code
 */
int delete_key(const int key);

/**
 * Checks if an item exists in the data structure
 *
 * @param key Key of the item to check
 * @return `1` if the item exist, `0` if it doesn't, or `-1` if there was an
 *      error
 */
int exist(const int key);

/**
 * Gets the data associated with an item
 *
 * @param key Key of the item to read
 * @param str Buffer in which to write the string value
 * @param vec_size Pointer to where the size of the vector should be written
 * @param vec Buffer in which to write the vector
 * @return Error code
 */
int get_value(const int key,
              char str[MAX_STR_SIZE],
              int* vec_size,
              double vec[MAX_VEC_SIZE]);

/**
 * Adds a new item to the data structure
 *
 * @param key Key of the item to add
 * @param str String value of the item
 * @param vec_size Vector value size
 * @param vec Vector value of the item
 * @return Error code
 */
int set_value(const int key,
              const char string[],
              const int vec_size,
              const double vec[vec_size]);

/**
 * Modifies an item in the data structure
 *
 * @param key Key of the item to modify
 * @param str New string value of the item
 * @param vec_size New vector value size
 * @param vec New vector value of the item
 * @return Error code
 */
int modify_value(const int key,
                 const char string[],
                 const int vec_size,
                 const double vec[vec_size]);

#endif // CLAVES_H
