#include <float.h>
#include <limits.h>
#include <math.h>

#include "../common/claves.h"
#include "tests.h"

#define MAX_ELEMENTS 1024

TEST(init_clear) {
    double vec[MAX_VEC_SIZE] = {1, 2, 3, 4};
    ASSERT_EQ(init(), 0);
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

TEST(set_multiple) {
    double vec[MAX_VEC_SIZE] = {1, 2, 3, 4};
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(set_value(0, "test", 4, vec), 0);
    ASSERT_EQ(set_value(0, "modified", 1, vec), -1);
    double vec_res[MAX_VEC_SIZE];
    int vec_size_res;
    char str_res[MAX_STR_SIZE];
    ASSERT_EQ(get_value(0, str_res, &vec_size_res, vec_res), 0);
    ASSERT_EQ(str_res, "test");
    ASSERT_EQ_VEC(vec_res, vec_size_res, vec, 4);
    return 0;
}

TEST(set_key_values) {
    double vec[MAX_VEC_SIZE] = {1, 2, 3, 4};
    int keys[5] = {0, 1, -1, INT_MAX, INT_MIN};
    double vec_res[MAX_VEC_SIZE];
    int vec_size_res;
    char str_res[MAX_STR_SIZE];
    ASSERT_EQ(init(), 0);
    for (int i = 0; i < 4; ++i) {
        ASSERT_EQ(set_value(keys[i], "test content", 4, vec), 0);
        ASSERT_EQ(get_value(keys[i], str_res, &vec_size_res, vec_res), 0);
        ASSERT_EQ(str_res, "test content");
        ASSERT_EQ_VEC(vec_res, vec_size_res, vec, 4);
    }
    return 0;
}

TEST(set_str_values) {
    double vec[MAX_VEC_SIZE] = {1, 2, 3, 4};
    char strs[4][MAX_STR_SIZE] = {"", "a", "aa", ""};
    memset(strs[3], 'a', sizeof(strs[3]) - 1);
    double vec_res[MAX_VEC_SIZE];
    int vec_size_res;
    char str_res[MAX_STR_SIZE];
    ASSERT_EQ(init(), 0);
    for (int i = 0; i < 4; ++i) {
        ASSERT_EQ(set_value(i, strs[i], 4, vec), 0);
        ASSERT_EQ(get_value(i, str_res, &vec_size_res, vec_res), 0);
        ASSERT_EQ(str_res, strs[i]);
        ASSERT_EQ_VEC(vec_res, vec_size_res, vec, 4);
    }

    char str2[MAX_STR_SIZE + 1] = {0};
    memset(str2, 'a', sizeof(str2) - 1);
    ASSERT_EQ(set_value(4, str2, 4, vec), -1);
    ASSERT_EQ(exist(4), 0);
    return 0;
}

TEST(set_vec_values) {
    double vec[MAX_ELEMENTS] = {
        1, 2, 3, 4, 0, -10, 0.5, NAN, INFINITY, -NAN, -INFINITY};
    int sizes[4] = {1, 2, 31, 32};
    double vec_res[MAX_VEC_SIZE];
    int vec_size_res;
    char str_res[MAX_STR_SIZE];
    ASSERT_EQ(init(), 0);
    for (int i = 0; i < 4; ++i) {
        ASSERT_EQ(set_value(i, "a", sizes[i], vec), 0);
        ASSERT_EQ(get_value(i, str_res, &vec_size_res, vec_res), 0);
        ASSERT_EQ(str_res, "a");
        ASSERT_EQ_VEC(vec_res, vec_size_res, vec, sizes[i]);
    }

    ASSERT_EQ(set_value(4, "a", 0, vec), -1);
    ASSERT_EQ(set_value(5, "a", 33, vec), -1);
    ASSERT_EQ(set_value(6, "a", 1024, vec), -1);
    // ASSERT_EQ(set_value(7, "a", -1, vec), -1); // Compilation error
    // ASSERT_EQ(set_value(8, "a", INT_MIN, vec), -1); // Compilation error
    for (int i = 4; i < 9; ++i) {
        ASSERT_EQ(exist(i), 0);
    }
    return 0;
}

TEST(set_get) {
    char str[MAX_STR_SIZE] = "test content";
    double vec[MAX_VEC_SIZE] = {1, 2, 3, 4};

    char str_res[MAX_STR_SIZE];
    int vec_size_res;
    double vec_res[MAX_VEC_SIZE];

    ASSERT_EQ(init(), 0);
    ASSERT_EQ(set_value(0, str, 4, vec), 0);
    ASSERT_EQ(get_value(0, str_res, &vec_size_res, vec_res), 0);
    ASSERT_EQ(str, str_res);
    ASSERT_EQ_VEC(vec_res, vec_size_res, vec, 4);
    return 0;
}

TEST(set_get_long_serialization) {
    char str[MAX_STR_SIZE] = "";
    memset(str, 'a', sizeof(str) - 1);
    double vec[MAX_VEC_SIZE];
    for (int i = 0; i < MAX_VEC_SIZE; ++i) {
        vec[i] = DBL_MAX;
    }

    char str_res[MAX_STR_SIZE];
    int vec_size_res;
    double vec_res[MAX_VEC_SIZE];

    ASSERT_EQ(init(), 0);
    ASSERT_EQ(set_value(0, str, MAX_VEC_SIZE, vec), 0);
    ASSERT_EQ(get_value(0, str_res, &vec_size_res, vec_res), 0);
    ASSERT_EQ(str, str_res);
    ASSERT_EQ_VEC(vec_res, vec_size_res, vec, MAX_VEC_SIZE);
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

TEST(get_empty) {
    double vec[MAX_VEC_SIZE];
    char str[MAX_STR_SIZE];
    int n_vec;

    ASSERT_EQ(init(), 0);
    ASSERT_EQ(get_value(3, str, &n_vec, vec), -1);
    return 0;
}

TEST(get_many_elements) {
    char strs[MAX_ELEMENTS][MAX_STR_SIZE];
    double vecs[MAX_ELEMENTS][MAX_VEC_SIZE];

    char strs_res[MAX_ELEMENTS][MAX_STR_SIZE];
    double vecs_res[MAX_ELEMENTS][MAX_VEC_SIZE];
    int n_vecs[MAX_ELEMENTS];

    ASSERT_EQ(init(), 0);

    // Insertion of 1024 elements.
    for (int i = 0; i < MAX_ELEMENTS; ++i) {
        snprintf(strs[i], sizeof(strs[i]), "test content: %d", i);
        vecs[i][0] = i;
        ASSERT_EQ(set_value(i, strs[i], 1, vecs[i]), 0);
    }

    // Get the vectors correctly.
    for (int i = 0; i < MAX_ELEMENTS; ++i) {
        ASSERT_EQ(get_value(i, strs_res[i], &n_vecs[i], vecs_res[i]), 0);
    }

    // Check the values are stored correctly.
    for (int i = 0; i < MAX_ELEMENTS; ++i) {
        ASSERT_EQ_VEC(vecs_res[i], n_vecs[i], vecs[i], 1);
        ASSERT_EQ(strs_res[i], strs[i]);
    }

    return 0;
}

TEST(modify_none) {
    double vec[MAX_VEC_SIZE] = {1, 2, 3, 4};
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(modify_value(5, "test", 4, vec), -1);
    ASSERT_EQ(exist(5), 0);
    return 0;
}

TEST(modify_element) {
    double vec[MAX_VEC_SIZE] = {3, 2, 4, 5};
    char str[MAX_STR_SIZE] = "modify_test";
    int n_value;

    ASSERT_EQ(init(), 0);
    ASSERT_EQ(set_value(0, str, 4, vec), 0);

    double vec2[MAX_VEC_SIZE] = {1, 4, 6};
    char str2[MAX_STR_SIZE] = "hola";

    ASSERT_EQ(modify_value(0, str2, 3, vec2), 0);
    ASSERT_EQ(get_value(0, str, &n_value, vec), 0);
    ASSERT_EQ_VEC(vec, n_value, vec2, 3);
    ASSERT_EQ(str, str2);

    double vec3[MAX_VEC_SIZE] = {1, 4, 6, 8, -1, 0, 10};
    char str3[MAX_STR_SIZE] = "very long string content for testing";

    ASSERT_EQ(modify_value(0, str3, 6, vec3), 0);
    ASSERT_EQ(get_value(0, str, &n_value, vec), 0);
    ASSERT_EQ_VEC(vec, n_value, vec3, 6);
    ASSERT_EQ(str, str3);
    return 0;
}

TEST(modify_str_values) {
    double vec[MAX_VEC_SIZE] = {1, 2, 3, 4};
    char strs[4][MAX_STR_SIZE] = {"", "a", "aa", ""};
    memset(strs[3], 'a', sizeof(strs[3]) - 1);
    double vec_res[MAX_VEC_SIZE];
    int vec_size_res;
    char str_res[MAX_STR_SIZE];
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(set_value(0, "test content", 4, vec), 0);
    for (int i = 0; i < 4; ++i) {
        ASSERT_EQ(modify_value(0, strs[i], 4, vec), 0);
        ASSERT_EQ(get_value(0, str_res, &vec_size_res, vec_res), 0);
        ASSERT_EQ(str_res, strs[i]);
        ASSERT_EQ_VEC(vec_res, vec_size_res, vec, 4);
    }

    char str2[MAX_STR_SIZE + 1] = {0};
    memset(str2, 'a', sizeof(str2) - 1);
    ASSERT_EQ(set_value(4, str2, 4, vec), -1);
    ASSERT_EQ(exist(4), 0);
    return 0;
}

TEST(modify_vec_values) {
    double start_vec[MAX_VEC_SIZE] = {100};
    double vec[MAX_ELEMENTS] = {
        1, 2, 3, 4, 0, -10, 0.5, NAN, INFINITY, -NAN, -INFINITY};
    int sizes[4] = {1, 2, 31, 32};
    double vec_res[MAX_VEC_SIZE];
    int vec_size_res;
    char str_res[MAX_STR_SIZE];
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(set_value(0, "test content", 4, start_vec), 0);
    for (int i = 0; i < 4; ++i) {
        ASSERT_EQ(modify_value(0, "a", sizes[i], vec), 0);
        ASSERT_EQ(get_value(0, str_res, &vec_size_res, vec_res), 0);
        ASSERT_EQ(str_res, "a");
        ASSERT_EQ_VEC(vec_res, vec_size_res, vec, sizes[i]);
    }

    ASSERT_EQ(modify_value(0, "a", 0, vec), -1);
    ASSERT_EQ(modify_value(0, "a", 33, vec), -1);
    ASSERT_EQ(modify_value(0, "a", 1024, vec), -1);
    // ASSERT_EQ(modify_value(0, "a", -1, vec), -1); // Compilation error
    // ASSERT_EQ(modify_value(0, "a", INT_MIN, vec), -1); // Compilation error
    ASSERT_EQ(get_value(0, str_res, &vec_size_res, vec_res), 0);
    ASSERT_EQ(str_res, "a");
    ASSERT_EQ_VEC(vec_res, vec_size_res, vec, sizes[3]);
    return 0;
}

TEST(exist_none) {
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(exist(0), 0);
    return 0;
}

TEST(exist_element) {
    double vec[MAX_VEC_SIZE] = {3, 2, 4, 5};
    char str[MAX_STR_SIZE] = "exist_test";
    int n_value = 4;
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(set_value(0, str, n_value, vec), 0);
    ASSERT_EQ(exist(0), 1);
    return 0;
}

TEST(delete_none) {
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(delete_key(0), -1);
    return 0;
}

TEST(delete_element) {
    double vec[MAX_VEC_SIZE] = {3, 2, 4, 5};
    char str[MAX_STR_SIZE] = "exist_test";
    int n_value = 4;
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(set_value(0, str, n_value, vec), 0);
    ASSERT_EQ(delete_key(0), 0);
    ASSERT_EQ(exist(0), 0);
    return 0;
}

TEST(delete_multiple) {
    double vec[MAX_VEC_SIZE] = {1, 2, 3, 4};
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(set_value(0, "test 1", 4, vec), 0);
    ASSERT_EQ(set_value(1, "test 2", 4, vec), 0);
    ASSERT_EQ(delete_key(0), 0);
    ASSERT_EQ(exist(0), 0);
    ASSERT_EQ(exist(1), 1);
    return 0;
}

TEST(delete_repeated) {
    double vec[MAX_VEC_SIZE] = {1, 2, 3, 4};
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(set_value(0, "test", 4, vec), 0);
    ASSERT_EQ(delete_key(0), 0);
    ASSERT_EQ(delete_key(0), -1);
    return 0;
}

int main() {
    CHECK(init());
    init_clear();
    init_multiple();
    set_key_values();
    set_str_values();
    set_vec_values();
    set_many_elements();
    set_get();
    set_get_long_serialization();
    get_empty();
    get_many_elements();
    modify_none();
    modify_element();
    modify_str_values();
    modify_vec_values();
    exist_none();
    exist_element();
    delete_none();
    delete_multiple();
    delete_repeated();
    return 0;
}
