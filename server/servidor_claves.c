#define _XOPEN_SOURCE 500
#include <errno.h>
#include <ftw.h>
#include <string.h>
#include <unistd.h>

#include "../common/claves.h"
#include "../common/error_handling.h"

// ╔═══════════════════════════════════════════════════════════════════════════╗
// ║                            Storage parameters                             ║
// ╚═══════════════════════════════════════════════════════════════════════════╝

/**
 * Directory in which the files will be stored
 */
const char* DATA_PATH = "data";
/**
 * Maximum path size of the files
 */
const int PATH_SIZE = 32;

// ╔═══════════════════════════════════════════════════════════════════════════╗
// ║                             Utility Functions                             ║
// ╚═══════════════════════════════════════════════════════════════════════════╝

/**
 * @brief Callback function to delete a file with a given path
 *
 * @param fpath Path to remove
 * @return Error code
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int unlink_cb(const char* fpath,
              const struct stat* sb,
              int typeflag,
              struct FTW* ftwbuf) {
    return remove(fpath);
}

/**
 * @brief Removes a directory and all of its contents
 *
 * @return Error code
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int remove_dir() {
    int ret = nftw(DATA_PATH, unlink_cb, 4, FTW_DEPTH | FTW_PHYS);
    if (ret != 0 && errno != ENOENT) {
        perror("Error deleting data folder");
        return -1;
    }
    return 0;
}

/**
 * @brief Gets the file path corresponding to the file with a given key
 *
 * @param path Buffer in which to write the path
 * @param key Key of the item
 * @return Pointer to `path`
 */
char* get_path(char path[PATH_SIZE], const int key) {
    snprintf(path, PATH_SIZE, "%s/%d", DATA_PATH, key);
    return path;
}

/**
 * Mode in which a file should be opened
 */
typedef enum {
    Read,   // Read only mode
    New,    // Create and write mode
    Update, // Overwrite mode
} file_mode;

/**
 * @brief Opens the file corresponding to the item with the given key
 *
 * @param key Key of the item
 * @param mode Mode in which the file should be opened
 * @return File pointer to the opened file
 * @retval NULL if there was an error
 */
FILE* get_file(const int key, const file_mode mode) {
    char path[PATH_SIZE];
    return fopen(get_path(path, key),
                 mode == Read ? "r" : (mode == Update ? "w" : "wr"));
}

/**
 * @brief Writes the values of an item to their corresponding file
 *
 * @param key Key of the item
 * @param str String value
 * @param vec_size Vector size
 * @param vec Vector value
 * @param mode Mode in which the contents should be written (`New` or `Update`)
 * @return Error code
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int write_file(const int key,
               const char str[],
               const int vec_size,
               const double vec[],
               const file_mode mode) {
    ASSERT(mode != Read, "Invalid file mode %d", mode)
    ASSERT(vec_size >= 1 && vec_size <= MAX_VEC_SIZE,
           "Invalid vector size %d",
           vec_size);
    int str_size = strlen(str);
    ASSERT(str_size <= MAX_STR_SIZE - 1, "Invalid string size %d", str_size)

    FILE* file =
        CHECK(get_file(key, mode), "Error opening data file for writing", NULL);
    CHECK(fwrite(str, sizeof(*str), str_size + 1, file) != str_size + 1,
          "Error writing string");
    CHECK(fwrite(&vec_size, sizeof(vec_size), 1, file) != 1,
          "Error writing vector size");
    CHECK(fwrite(vec, sizeof(*vec), vec_size, file) != vec_size,
          "Error writing vector");
    CHECK(fclose(file), "Error closing file after write");

    return 0;
}

// ╔═══════════════════════════════════════════════════════════════════════════╗
// ║                            API Implementation                             ║
// ╚═══════════════════════════════════════════════════════════════════════════╝

int init() {
    CHECK(remove_dir());
    CHECK(mkdir(DATA_PATH, 0744), "Error creating data folder");
    return 0;
}

int delete_key(const int key) {
    char path[PATH_SIZE];
    CHECK(unlink(get_path(path, key)), "Error deleting key file");
    return 0;
}

int exist(const int key) {
    FILE* file = get_file(key, Read);
    if (file != NULL) {
        CHECK(fclose(file));
        return 1;
    }
    if (ENOENT == errno) { return 0; }
    perror("Error checking existence of file");
    return -1;
}

int get_value(const int key,
              char str[MAX_STR_SIZE],
              int* vec_size,
              double v[MAX_VEC_SIZE]) {
    FILE* file =
        CHECK(get_file(key, Read), "Error opening data file for reading", NULL);
    int i = 0;
    do {
        ASSERT(fread(&str[i], sizeof(*str), 1, file) == 1,
               "Error reading string");
    } while (str[i++] != '\0');
    ASSERT(fread(vec_size, sizeof(*vec_size), 1, file) == 1,
           "Error reading vector size");
    ASSERT(fread(v, sizeof(*v), *vec_size, file) == *vec_size,
           "Error reading vector");
    CHECK(fclose(file), "Error closing file after reading");

    return 0;
}

int set_value(const int key,
              const char string[],
              const int vec_size,
              const double vec[vec_size]) {
    return write_file(key, string, vec_size, vec, New);
}

int modify_value(const int key,
                 const char string[],
                 const int vec_size,
                 const double vec[vec_size]) {
    return write_file(key, string, vec_size, vec, Update);
}
