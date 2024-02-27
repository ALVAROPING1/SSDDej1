#ifndef CLAVES_H
#define CLAVES_H

// Max size of value 1 (string)
#define MAX_STR_SIZE 255
// Max size of value 2 (float vector)
#define MAX_VEC_SIZE 32

int init();

int delete_key(const int key);

int exist(const int key);

int get_value(const int key,
              char str[MAX_STR_SIZE],
              int* vec_size,
              double vec[MAX_VEC_SIZE]);

int set_value(const int key,
              const char string[],
              const int vec_size,
              const double vec[vec_size]);

int modify_value(const int key,
                 const char string[],
                 const int vec_size,
                 const double vec[vec_size]);

#endif // CLAVES_H
