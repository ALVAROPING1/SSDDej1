#include <limits.h>

#include "../common/claves.h"
#include "tests.h"

#define MAX_ELEMENTS 1024

TEST(init_clear) {
    ASSERT_EQ(init(), 0);
    double vec[MAX_VEC_SIZE] = {1, 2, 3, 4};
    ASSERT_EQ(set_value(0, "test", 4, vec), 0);
    ASSERT_EQ(exist(0), 1);
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(exist(0), 0);
    return 0;
}

TEST(init_multiple) {
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(init(), 0);
    return 0;
}

TEST(set_key_values) {
    ASSERT_EQ(init(), 0);
    double vec[MAX_VEC_SIZE] = {1, 2, 3, 4};
    ASSERT_EQ(set_value(0, "test", 4, vec), 0);
    ASSERT_EQ(set_value(-1, "test", 4, vec), 0);
    ASSERT_EQ(set_value(INT_MAX, "test", 4, vec), 0);
    ASSERT_EQ(set_value(INT_MIN, "test", 4, vec), 0);
    return 0;
}

TEST(set_str_values) {
    ASSERT_EQ(init(), 0);
    double vec[MAX_VEC_SIZE] = {1, 2, 3, 4};
    ASSERT_EQ(set_value(0, "", 4, vec), 0);
    ASSERT_EQ(set_value(1, "a", 4, vec), 0);
    ASSERT_EQ(set_value(2, "aa", 4, vec), 0);

    char str1[MAX_STR_SIZE] = {0};
    memset(str1, 'a', sizeof(str1) - 1);
    ASSERT_EQ(set_value(3, str1, 4, vec), 0);

    char str2[MAX_STR_SIZE + 1] = {0};
    memset(str2, 'a', sizeof(str2) - 1);
    ASSERT_EQ(set_value(4, str2, 4, vec), -1);
    return 0;
}

TEST(set_vec_values) {
    ASSERT_EQ(init(), 0);
    double vec[MAX_ELEMENTS] = {1, 2, 3, 4};
    ASSERT_EQ(set_value(0, "a", 0, vec), -1);
    ASSERT_EQ(set_value(1, "a", 1, vec), 0);
    ASSERT_EQ(set_value(2, "a", 2, vec), 0);
    ASSERT_EQ(set_value(3, "a", 31, vec), 0);
    ASSERT_EQ(set_value(4, "a", 32, vec), 0);
    ASSERT_EQ(set_value(5, "a", 33, vec), -1);
    ASSERT_EQ(set_value(8, "a", 1024, vec), -1);
    // ASSERT_EQ(set_value(6, "a", -1, vec), -1); // Error de compilación
    // ASSERT_EQ(set_value(7, "a", INT_MIN, vec), -1); // Error de compilación
    return 0;
}

TEST(set_get) {
    ASSERT_EQ(init(), 0);
    char str[MAX_STR_SIZE] = "test content";
    double vec[MAX_VEC_SIZE] = {1, 2, 3, 4};

    char str_res[MAX_STR_SIZE];
    int vec_size_res;
    double vec_res[MAX_VEC_SIZE];

    ASSERT_EQ(set_value(0, str, 4, vec), 0);
    ASSERT_EQ(get_value(0, str_res, &vec_size_res, vec_res), 0);
    ASSERT_EQ(str, str_res);
    ASSERT_EQ_VEC(vec_res, vec_size_res, vec, 4);
    return 0;
}

TEST(set_many_elements) {
    double vec[4] = {1, 2, 3, 4};
    ASSERT_EQ(init(), 0);
    for (int i = 0; i < MAX_ELEMENTS; ++i) {
        ASSERT_EQ(set_value(i, "test", 4, vec), 0);
    }
    return 0;
}

TEST(get_empty){
    double vec[MAX_VEC_SIZE];
    char str[MAX_STR_SIZE];
    int n_vec;

    ASSERT_EQ(init(), 0);
    ASSERT_EQ(get_value(3, str, &n_vec, vec), -1);

    return 0;
}

TEST(get_many_elements){
    double vec[MAX_VEC_SIZE] = {1, 2, 3, 4};
    
    double vecs_res[MAX_ELEMENTS][MAX_VEC_SIZE];
    char strs[MAX_ELEMENTS][MAX_STR_SIZE];
    int n_vecs[MAX_ELEMENTS];

    ASSERT_EQ(init(), 0);

    // Insertion of 1024 elements.
    for (int i = 0; i < MAX_ELEMENTS; ++i) {
        ASSERT_EQ(set_value(i, "test", 4, vec), 0);
    }

    // Get the vectors correctly.
    for (int i = 0; i < MAX_ELEMENTS; ++i) {
      ASSERT_EQ(get_value(i,strs[i],n_vecs+i,vecs_res[i]), 0);
    }

    // Check the values are stored correctly.
    for (int i = 0; i < MAX_ELEMENTS; ++i) {
      ASSERT_EQ_VEC(vecs_res[i], n_vecs[i], vec, 4);
      ASSERT_EQ(strs[i], "test");
    }
    
    return 0;
}

TEST(modify_none){
    double vec[MAX_VEC_SIZE] = {1,2,3,4};
    
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(modify_value(5,"test",4,vec), -1);
    return 0;
}

TEST(modify_element){
    double vec[MAX_VEC_SIZE] = {3,2,4,5};
    char str[MAX_STR_SIZE] = "modify_test";
    int n_value;
    
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(set_value(0, str, 4, vec), 0);

    double vec2[MAX_VEC_SIZE] = {1,4,6,8};
    char str2[MAX_STR_SIZE] = "hola";

    ASSERT_EQ(modify_value(0, str2, 4, vec2), 0);

    ASSERT_EQ(get_value(0,str,&n_value,vec), 0);

    ASSERT_EQ_VEC(vec, n_value, vec2, 4);
    ASSERT_EQ(str, str2);
  
  return 0;
}

TEST(exist_none){
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(exist(0), 0);
    
    return 0;
}

TEST(exist_element){
    ASSERT_EQ(init(), 0);
    
    double vec[MAX_VEC_SIZE] = {3,2,4,5};
    char str[MAX_STR_SIZE] = "exist_test";
    int n_value = 4;

    ASSERT_EQ(set_value(0, str, n_value, vec), 0);
    ASSERT_EQ(exist(0), 1);

    return 0;
    
}

TEST(delete_none){
    ASSERT_EQ(init(), 0);
    
    ASSERT_EQ(delete_key(0), -1);
    
    return 0;
}

TEST(delete_element){
    ASSERT_EQ(init(), 0);
    
    double vec[MAX_VEC_SIZE] = {3,2,4,5};
    char str[MAX_STR_SIZE] = "exist_test";
    int n_value = 4;
    
    ASSERT_EQ(set_value(0, str, n_value, vec), 0);
    ASSERT_EQ(exist(0), 1);
    ASSERT_EQ(delete_key(0), 0);
    ASSERT_EQ(exist(0), 0);

    return 0;
}

int main() {
    init_clear();
    init_multiple();
    set_key_values();
    set_str_values();
    set_vec_values();
    set_many_elements();
    set_get();
    get_empty();
    get_many_elements();
    modify_element();
    exist_none();
    exist_element();
    delete_none();
    delete_element();
    return 0;
}
