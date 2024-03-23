#include <stdlib.h>

#include "error_handling.h"
#include "utils.h"

int string2int(const char* string, int* destination) {
    char* leftover;
    // Tries to convert the string to an int
    *destination = strtol(string, &leftover, 10);
    // If the entire string was consumed in the conversion,
    // the leftover string should be empty
    CHECK(leftover[0] != '\0');
    return 0;
}
