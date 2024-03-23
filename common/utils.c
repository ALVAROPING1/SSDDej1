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

int getServerIp(char** ip) {
    *ip = CHECK(getenv("IP_TUPLAS"),
                "Error getting server IP environment variable",
                NULL);
    return 0;
}

int getServerPort(int* port) {
    char* var = getenv("PORT_TUPLAS");
    CHECK(var, "Error getting server port environment variable", NULL);
    CHECK(string2int(var, port),
          "Error converting server port environment variable to an int");
    return 0;
}
