#include <stdlib.h>
#include <string.h>

#include "./env.h"
#include "./error_handling.h"
#include "./utils.h"

int getServerIp(char* ip) {
    char* var = getenv("IP_TUPLAS");
    CHECK(var, "Error getting server IP environment variable", NULL);
    strcpy(ip, var);
    return 0;
}

int getServerPort(int* port) {
    char* var = getenv("PORT_TUPLAS");
    CHECK(var, "Error getting server port environment variable", NULL);
    CHECK(string2int(var, port),
          "Error converting server port environment variable to an int");
    return 0;
}
