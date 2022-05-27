#ifndef STRING_PARSERS_H
#define STRING_PARSERS_H

typedef struct {
    int length;
    char** strings;
} StrArr;

char* rtrim(char*);
int is_white_space(char);
char* ltrim(char*);
char* trim(char* s);
char* substring(char* target, const char* source, int start, int n);
StrArr* split(char* string, char* delimiter);
StrArr* new_str_arr();
char* get_str_at_index(StrArr* array, int index);
StrArr* get_line_words(char* line);
void free_str_arr(StrArr* arr);
char* cp_string(char* str);
char* cat_string(char* target, char* str_to_append);

#endif
