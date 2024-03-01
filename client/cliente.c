#include <limits.h>

#include "../common/claves.h"
#include "tests.h"

TEST(init1) {
    ASSERT_EQ(init(), 0);
    double vec[32] = {1, 2, 3, 4};
    ASSERT_EQ(set_value(0, "test", 4, vec), 0);
    ASSERT_EQ(init(), 0);
    char str[256];
    int vec_size;
    ASSERT_EQ(get_value(0, str, &vec_size, vec), -1);
    return 0;
}

TEST(init2) {
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(init(), 0);
    return 0;
}

TEST(set_key_values) {
    ASSERT_EQ(init(), 0);
    double vec[32] = {1, 2, 3, 4};
    ASSERT_EQ(set_value(0, "test", 4, vec), 0);
    ASSERT_EQ(set_value(-1, "test", 4, vec), 0);
    ASSERT_EQ(set_value(INT_MAX, "test", 4, vec), 0);
    ASSERT_EQ(set_value(INT_MIN, "test", 4, vec), 0);
    return 0;
}

TEST(set_str_values) {
    ASSERT_EQ(init(), 0);
    double vec[32] = {1, 2, 3, 4};
    ASSERT_EQ(set_value(0, "", 4, vec), 0);
    ASSERT_EQ(set_value(1, "a", 4, vec), 0);
    ASSERT_EQ(set_value(2, "aa", 4, vec), 0);
    char str1[MAX_STR_SIZE] = {0};
    memset(str1, 'a', sizeof(str1) - 1);
    ASSERT_EQ(set_value(3, str1, 4, vec), 0);
    char str2[MAX_STR_SIZE + 1] = {0};
    memset(str2, 'a', sizeof(str2));
    ASSERT_EQ(set_value(4, str2, 4, vec), -1);
    return 0;
}

TEST(set_vec_values) {
    ASSERT_EQ(init(), 0);
    double vec[1024] = {1, 2, 3, 4};
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
    char str[256] = {'a', 0};
    char str_res[256] = {'a', 0};
    int vec_size = 4;
    int vec_size_res = 4;
    double vec[32] = {1, 2, 3, 4};
    double vec_res[32] = {1, 2, 3, 4};
    ASSERT_EQ(set_value(0, str, vec_size, vec), 0);
    ASSERT_EQ(get_value(0, str, &vec_size_res, vec_res), 0);
    ASSERT_EQ(str, str_res);
    ASSERT_EQ_VEC(vec, vec_size, vec_res, vec_size_res);
    return 0;
}

TEST(set_many_elements) {
    double vec[4] = {1, 2, 3, 4};
    ASSERT_EQ(init(), 0);
    for (int i = 0; i < 1024; ++i) {
        ASSERT_EQ(set_value(i, "test", 4, vec), 0);
    }
    return 0;
}

int main() {
    init1();
    init2();
    set_key_values();
    set_str_values();
    set_vec_values();
    set_many_elements();
    set_get();
    return 0;
}
