#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

#include "./str_table.h"

typedef struct {
  char *name;
  StrTable *body;
} Macro;

typedef struct macroNode {
  Macro *macro;
  struct macroNode *next;
} MacroNode;

typedef struct {
  MacroNode *head;
} MacroList;

typedef struct {
  MacroList *macros;
  int macro_count;
} MacroTable;

/* creates a new allocation to Macro node and assingen default values, returns
 * the pointer */
MacroNode *get_new_macro_node(Macro *);
void add_macro_to_table(Macro *macro, MacroTable *macroTable);
void add_macro_to_list(Macro *, MacroList *);
/* creates a new allocation to Macro and assingen default values, returns the
 * pointer */
Macro *get_new_macro(char *name);
/* frees the pointr to macro provided and all assosiated allocated memory */
void free_macro(Macro *);
MacroNode *get_macro_node_by_name_from_list(char *, MacroList *);
int is_macro_in_table(char *, MacroTable *);
/* creates a new allocation to Macro list and assingen default values, returns
 * the pointer */
MacroList *init_macro_list();
/* creates a new allocation to Macro table and assingen default values, returns
 * the pointer */
MacroTable *init_macro_table();
/* frees the pointr to macro node provided and all assosiated allocated memory
 */
void free_macro_node(MacroNode *);
/* frees the pointr to macro list provided and all assosiated allocated memory
 */
void free_macro_list(MacroList *);
/* frees the pointr to macro table provided and all assosiated allocated memory
 */
void clear_macro_table(MacroTable *);
Macro *get_macro_by_name_from_table(char *macro_name, MacroTable *macro_table);
/* spreads macro to print_target */
void print_macro(Macro *macro, FILE *print_target);

#endif
