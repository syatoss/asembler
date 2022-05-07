#include <stdio.h>
#include "./str_linked_list.h"

typedef struct {
    int num_of_lines;
    str_linked_list* strings;
    FILE* print_target;
} str_table;


void add_str(str_table*,char*);
void println_str(str_table*,char*);
void print_table(str_table*);
void clear_str_table(str_table*);
str_table* new_str_table(FILE*);
