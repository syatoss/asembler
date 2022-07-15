#include "./../headers/string_parsers.h"

#include <stdarg.h>
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

char* cat_strings(char* target, ...) {
    va_list string_to_concat;
    char* concatenated = NULL;
    char* next_string;
    va_start(string_to_concat, target);
    concatenated = cp_string(target);
    while ((next_string = va_arg(string_to_concat, char*)))
        concatenated = cat_string(concatenated, next_string);
    va_end(string_to_concat);
    return concatenated;
}

char* cat_string(char* target, char* str_to_append) {
    char* appended = cp_string(target);
    appended = (char*)realloc(
        appended,
        sizeof(char) * (strlen(appended) + strlen(str_to_append) + 1));
    if (!appended) SYS_MEM_FAIL_EXIT(1);
    appended = strcat(appended, str_to_append);
    free(target);
    return appended;
}

void free_str_arr(StrArr* arr) {
    int i = 0;
    while (i < arr->length) free(arr->strings[i++]);
    free(arr->strings);
    free(arr);
}

char* cp_string(char* str) {
    char* str_cp;
    str_cp = str ? (char*)malloc(sizeof(char*) * (strlen(str) + 1))
                 : (char*)malloc(sizeof(char));
    str ? strcpy(str_cp, str) : strcpy(str_cp, "");
    return str_cp;
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
