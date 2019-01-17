#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// ============================================== STRING API ============================================= //

// return:  returns the index of the '\0' character!
static size_t str_length(char* s) {
    size_t length = 0;
    while (s[length++]);
    return length;
}

// str:     original string
// return:  heap allocated copy of 'str'
static char* str_copy(char* str) {
    char*  copy   = NULL;
    size_t length = str_length(str);

    if (length > 0) {
        copy = malloc(sizeof (char) * (length + 1));

        for (int i = 0; i < length + 1; ++i)
            copy[i] = str[i];
    }

    return copy;
}

// str:     string to search!
// token:   character to find!
// return:  1: if token is in string, 0: otherwise
static int str_contains(char* str, char token) {
    char c;
    while (c = *(str++)) if (c == token) return 1;
    return 0;
}

// a:       first  string
// b:       second string
// return:  1: if they are the same, 0: otherwise
static int str_eq(char* a, char* b) {
    while (*a && *b) if (*(a++) != *(b++)) return 0;
    return *a == *b;
}

// taken from stb libs:
size_t str_hash(char *str) {
    size_t hash = 0;
    while (*str)
        hash = (hash << 7) + (hash >> 25) + *str++;
    return hash + (hash >> 16);
}


// @NOTE: modifies the shit out of the original string. so pass in a copy if you want to keep it!!!
// str:     string to be splitted!
// del:     tokens that will split the string!
// rcount:  output variable that will hold the length of the reslut array!
// return:  array of pointers to the original string where all the delimiters has been replaced with NULL characters
static char** str_split(char* str, char* del, size_t* rcount) {
    size_t  capacity = 0;
    size_t  count    = 0;
    char**  result   = NULL;

    for (int i = 0; str[i] != '\0'; ++i) {
        if (str_contains(del, str[i])) {
            str[i] = '\0';
        } else if (str[i] != '\0' && (i == 0 || str[i - 1] == '\0')) {
            count++;

            if (count >= capacity) {
                capacity <<= 1;
                capacity = count > capacity? count : capacity;
                result   = realloc(result, capacity * sizeof *result);
            }
            result[count - 1] = &str[i];
        }
    }
    *rcount = count;
    return result;
}

// fn:      the name of the file!
// return:  the content of the file, as a string!
static char* str_read_file(const char* fn) {
    assert(fn);

    FILE* fp = fopen(fn, "rb");
    assert(fp);

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    char* str = malloc((size + 1) * sizeof *str);
    assert(str);

    size_t result = fread(str, sizeof *str, size, fp);
    assert(size == result);

    str[size] = '\0';

    fclose(fp);

    return str;
}

