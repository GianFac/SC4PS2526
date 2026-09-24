/* =====================================================================
 * parser.h
 *
 * Vendored, unmodified copy of the single-header config-file parser
 * from https://github.com/welljsjs/Config-Parser-C (AGPL-3.0), as
 * suggested by the HomeWork06 assignment sheet:
 *   "use already existing libraries to read a configuration file
 *    parser, e.g.: https://github.com/welljsjs/Config-Parser-C"
 *
 * Reads a text file of "key = value" lines (with '#' comment lines)
 * into a singly-linked list of config_option nodes, each holding the
 * key and value as strings. Requires <stdio.h>, <stdlib.h>, <string.h>
 * to already be included by the caller.
 * =====================================================================
 */

#define CONFIG_ARG_MAX_BYTES 128

typedef struct config_option config_option;
typedef config_option* config_option_t;

struct config_option {
    config_option_t prev;
    char key[CONFIG_ARG_MAX_BYTES];
    char value[CONFIG_ARG_MAX_BYTES];
};

config_option_t read_config_file(char* path) {
    FILE* fp;
    if ((fp = fopen(path, "r+")) == NULL) {
        perror("fopen()");
        return NULL;
    }

    config_option_t last_co_addr = NULL;
    while(1) {
        config_option_t co = NULL;
        if ((co = calloc(1, sizeof(config_option))) == NULL)
            continue;
        memset(co, 0, sizeof(config_option));
        co->prev = last_co_addr;
        if (fscanf(fp, "%s = %s", &co->key[0], &co->value[0]) != 2) {
            if (feof(fp)) {
                break;
            }
            if (co->key[0] == '#') {
                while (fgetc(fp) != '\n') {
                    // Do nothing (to move the cursor to the end of the line).
                }
                free(co);
                continue;
            }
            perror("fscanf()");
            free(co);
            continue;
        }
        //printf("Key: %s\nValue: %s\n", co->key, co->value);
        last_co_addr = co;
    }
    fclose(fp);
    return last_co_addr;
}
