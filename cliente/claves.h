#ifndef CLAVES_H
#define CLAVES_H 1

int init_service();
int set_value(int key, char *val1, int n_value2, double *value2);
int get_value(int key, char *val1, int *n_value2, double *value2);
int modify_value(int key, char *val1, int n_value, double *value2);
int delete_key(int key);
int exist(int key);
int exit_service();

#endif // !CLAVES_H

