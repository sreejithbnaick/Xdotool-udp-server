/*
 * string_utils.c
 *
 *  Created on: 02-Apr-2016
 *      Author: sreejith
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "string_utils.h"

struct SplitResult str_split(char* a_str, const char a_delim) {
    if (a_str[0] == a_delim) {
        a_str++;
    }
    int size = strlen(a_str);
    if (a_str[size - 1] == a_delim) {
        a_str[size - 1] = '\0';
    }
    //printf("Splitting '%s', using '%c'\n",a_str,a_delim);
    struct SplitResult split_result;
    char** result = 0;
    size_t count = 0;
    char* tmp = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
     knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof (char*) * count);

    if (result) {
        size_t idx = 0;
        char* token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    split_result.tokens = result;
    split_result.count = count - 1;
    return split_result;
}

int str_contains_duplicate(char* a_str, const char a_delim) {
    char* tmp = a_str;
    int count = 0;
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
        }
        tmp++;
    }
    if (count > 1)
        return 1;
    else
        return 0;
}

char *str_concat(char *first, char *second) {
    size_t len1 = strlen(first);
    size_t len2 = strlen(second);

    char * s = malloc(len1 + len2 + 2);
    memcpy(s, first, len1);
    memcpy(s + len1, second, len2 + 1);
    return s;
}

