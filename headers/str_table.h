#ifndef STR_TABLE_H
#define STR_TABLE_H

#include <stdio.h>

#include "./str_linked_list.h"

typedef struct {
  int num_of_lines;
  StrList *strings;
} StrTable;

/* adds the string to the table */
void add_str(StrTable *, char *);
/* prints the table strings to the provided target */
void print_table(StrTable *, FILE *);
/* prints the provided string to the target file */
void println_str(StrTable *, char *, FILE *);
/* frees the pointer and all assosiated allocated memory */
void clear_str_table(StrTable *);
/* allocates new memory for StrTable assing defaults and returns the pointer */
StrTable *new_str_table();

#endif
