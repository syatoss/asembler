#include <stdio.h>
#include "./str_linked_list.h"

typedef struct {
    int num_of_lines;
    StrList* strings;
} StrTable;


void add_str(StrTable*,char*);
void print_table(StrTable*, FILE*);
void println_str(StrTable* ,char* , FILE* );
void clear_str_table(StrTable*);
StrTable* new_str_table();
