#include <string.h>
#include "./../headers/utils.h"
#include "./../headers/string_parsers.h"


char* ltrim(char* s) {
    while(is_white_space(*s)) s++;
    return s;
}

int is_white_space(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

char* rtrim(char* s) {
    int original_len = strlen(s);
    char last_char = *(s + original_len);
    char* back = s + original_len;
    while(is_white_space(*(--back)));
    if(last_char == '\n')
        *(++back) = last_char;
    back++;
    *(back) = '\0';
    return s;
}

char* trim(char* s) {
    return rtrim(ltrim(s));
}

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
