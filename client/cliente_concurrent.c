#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../common/claves.h"
#include "tests.h"

#define MAX_ELEMENTS 1024
#define PROCESSES 16

int concurrent_test(int (*func)()) {
    pid_t pids[PROCESSES];
    for (int i = 0; i < PROCESSES; ++i) {
        pids[i] = fork();
        if (0 == pids[i]) {
            // Child process
            exit(func());
        }
    }
    // Wait for all tests
    int results[PROCESSES];
    for (int i = 0; i < PROCESSES; ++i) {
        waitpid(pids[i], &results[i], 0);
    }
    // Check that there weren't any errors
    for (int i = 0; i < PROCESSES; ++i) {
        ASSERT_EQ(results[i], 0);
    }
    // Clear files
    ASSERT_EQ(init(), 0);
    return 0;
}

int get_many_elements() {
    char strs[MAX_ELEMENTS][MAX_STR_SIZE];
    double vecs[MAX_ELEMENTS][MAX_VEC_SIZE];

    char strs_res[MAX_ELEMENTS][MAX_STR_SIZE];
    double vecs_res[MAX_ELEMENTS][MAX_VEC_SIZE];
    int n_vecs[MAX_ELEMENTS];

    pid_t pid = getpid();
    int start = pid * MAX_ELEMENTS;

    // Insertion of 1024 elements.
    for (int i = 0; i < MAX_ELEMENTS; ++i) {
        snprintf(strs[i], sizeof(strs[i]), "test content: %d", i);
        vecs[i][0] = i;
        ASSERT_EQ(set_value(start + i, strs[i], 1, vecs[i]), 0);
    }
    printf("Process %d: all values set\n", pid);

    // Get the vectors correctly.
    for (int i = 0; i < MAX_ELEMENTS; ++i) {
        ASSERT_EQ(get_value(start + i, strs_res[i], &n_vecs[i], vecs_res[i]),
                  0);
    }
    printf("Process %d: all values gotten\n", pid);

    // Check the values are stored correctly.
    for (int i = 0; i < MAX_ELEMENTS; ++i) {
        ASSERT_EQ_VEC(vecs_res[i], n_vecs[i], vecs[i], 1);
        ASSERT_EQ(strs_res[i], strs[i]);
    }

    return 0;
}

TEST(set_get_concurrent) {
    ASSERT_EQ(init(), 0);
    ASSERT_EQ(concurrent_test(get_many_elements), 0);
    return 0;
}

int modify_element() {
    char str[MAX_STR_SIZE];
    double vec[MAX_VEC_SIZE];
    pid_t pid = getpid();
    for (int i = 0; i < MAX_STR_SIZE - 1; ++i) {
        // Writes all odd byte codes. This ensures that the NULL terminating
        // character isn't written and thus the length is always `MAX_STR_SIZE`
        str[i] = ((pid + i) * 2 + 1) % 256;
    }
    str[MAX_STR_SIZE - 1] = '\0';
    // Initializes the vector to the pid followed by unique numbers for each
    // process calculated from its pid
    vec[0] = pid;
    for (int i = 1; i < MAX_VEC_SIZE; ++i) {
        // Writes consecutive numbers from the reserved number space
        vec[i] = pid * MAX_VEC_SIZE + i;
    }
    ASSERT_EQ(modify_value(0, str, 32, vec), 0);
    printf("Process %d: value modified\n", pid);

    char str_res[MAX_STR_SIZE];
    double vec_res[MAX_VEC_SIZE];
    int vec_size_res;

    // Attempts to read the data until it succeeds
    while (get_value(0, str_res, &vec_size_res, vec_res) != 0) {}
    printf("Process %d: value gotten\n", pid);

    // Generates the correct data that should be stored according to the first
    // value (since they should follow a pattern). We can't just compare with
    // the data we written because it might have been overwritten by a different
    // concurrent process, so we must validate the integrity of the data gotten
    // by checking that it follows the correct pattern
    for (int i = 0; i < MAX_STR_SIZE - 1; ++i) {
        str[i] = (str_res[0] + 2 * i) % 256;
    }
    str_res[MAX_STR_SIZE - 1] = '\0';
    vec[0] = vec_res[0];
    for (int i = 1; i < MAX_VEC_SIZE; ++i) {
        vec[i] = vec[0] * MAX_VEC_SIZE + i;
    }
    ASSERT_EQ(str_res, str);
    ASSERT_EQ_VEC(vec_res, vec_size_res, vec, 32);
    return 0;
}

TEST(modify_concurrent) {
    double vec[MAX_VEC_SIZE] = {1};
    char str[MAX_STR_SIZE] = "empty";

    ASSERT_EQ(init(), 0);
    ASSERT_EQ(set_value(0, str, 1, vec), 0);
    ASSERT_EQ(concurrent_test(modify_element), 0);
    return 0;
}

int main() {
    CHECK(init());
    set_get_concurrent();
    modify_concurrent();
    return 0;
}
