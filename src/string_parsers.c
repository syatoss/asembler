#include "./../headers/string_parsers.h"

#include <stdlib.h>
#include <string.h>

#include "./../headers/utils.h"

char* ltrim(char* s) {
    while (is_white_space(*s)) s++;
    return s;
}

StrArr* new_str_arr() {
    StrArr* arr;
    arr = (StrArr*)malloc(sizeof(StrArr));
    if (!arr) SYS_MEM_FAIL_EXIT(1);
    arr->strings = NULL;
    arr->length = 0;
    return arr;
}

void free_str_arr(StrArr* arr) {
    int i = 0;
    while (i < arr->length) free(arr->strings[i++]);
    free(arr->strings);
    free(arr);
}

void push_next_string(StrArr* array, char* str) {
    char* str_cp;
    char** new_string_local;
    str_cp = (char*)malloc(sizeof(char) * (strlen(str) + 1));
    if (!str_cp) SYS_MEM_FAIL_EXIT(1);
    new_string_local =
        array->strings != NULL
            ? (char**)realloc(array->strings,
                              sizeof(char*) * (array->length + 1))
            : (char**)malloc(sizeof(char*) * (array->length + 1));
    array->strings = new_string_local;
    str_cp = strcpy(str_cp, str);
    array->strings[array->length] = str_cp;
    array->length++;
}

StrArr* get_line_words(char* line) { return split(line, " "); }

char* get_str_at_index(StrArr* array, int index) {
    if (index >= array->length) {
        fprintf(stderr,
                "Invalid use of function \"get_str_at_index\" index exceeds "
                "the array length");
        exit(2);
    }
    return array->strings[index];
}

StrArr* split(char* str, char* delimiter) {
    StrArr* arr;
    char* token;
    char* pivot;
    char* str_cp = (char*)malloc(sizeof(char) * (strlen(str) + 1));
    str_cp = strcpy(str_cp, str);
    pivot = str_cp;
    str_cp = trim(str_cp);
    arr = new_str_arr();
    token = strtok(str_cp, delimiter);
    while (token != NULL) {
        push_next_string(arr, trim(token));
        token = strtok(NULL, delimiter);
    }
    free(pivot);
    return arr;
}

int is_white_space(char c) { return c == ' ' || c == '\t' || c == '\n'; }

char* rtrim(char* s) {
    int original_len = strlen(s);
    char last_char = *(s + original_len);
    char* back = s + original_len;
    while (is_white_space(*(--back)))
        ;
    if (last_char == '\n') *(++back) = last_char;
    back++;
    *(back) = '\0';
    return s;
}

char* trim(char* s) { return rtrim(ltrim(s)); }

char* substring(char* target, const char* source, int start, int n) {
    while (n > 0) {
        *target = *(source + start);
        target++;
        source++;
        n--;
    }
    *target = '\0';
    return target;
}
