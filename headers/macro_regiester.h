#ifndef MACRO_REGISTER_H
#define MACRO_REGISTER_H

#include "../headers/asm_descriptor.h"
#include "../headers/macro_table.h"
#define MACRO_NAME_WORD_INDEX 1

int is_macro_def(char *);
int is_macro_def_end(char *);
char *get_macro_name_from_line(char *line);
/* returns 0 if not equal and non 0 if equal */
int nth_word_at_line_equals(char *line, int index, char *word_to_compare);
int line_has_macro_def(char *line, MacroTable *table);
Macro *register_new_macro_by_name(char *macro_name, MacroTable *macro_table);
/* adds the line to macro object for future spread */
Macro *register_line_to_macro(char *line, Macro *macro);
/* adds macro to macro table */
MacroTable *register_macro_to_table(Macro *macro, MacroTable *macro_table);
/* moves the pointer of the current file in AsmDescriptor, past the current
 * macro definition */
void skip_macro_def(AsmDescriptor *ds);

#endif
