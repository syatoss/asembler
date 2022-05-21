#include "../headers/string_parsers.h"
#include "../headers/constants.h"
#include "../headers/utils.h"
#include "../headers/macro_regiester.h"
#include <stdlib.h>
#include <string.h>


int is_macro_def(char* line) {
    return compare_first_word_to(line, MACRO_START);
}

int is_macro_def_end(char* line) {
    return compare_first_word_to(line, MACRO_END);
}

int compare_first_word_to(char* line, char* word_to_compare) {
    int trimmed_len;
    int compare_word_len;
    char* first_word;
    line = strcpy(line,trim(line));
    trimmed_len = strlen(line);
    compare_word_len = strlen(word_to_compare);
    if(trimmed_len < compare_word_len ) return false;
    first_word = malloc(sizeof(char) * compare_word_len );
    if(!first_word) SYS_MEM_FAIL_EXIT(1);
    first_word = substring(first_word, line, 0, compare_word_len );
    return strcmp(word_to_compare, first_word) == 0;
}
