#ifndef MACRO_REGISTER_H
#define MACRO_REGISTER_H

#include "../headers/asm_descriptor.h"
#include "../headers/macro_table.h"
#define MACRO_NAME_WORD_INDEX 1

int is_macro_def(char*);
int is_macro_def_end(char*);
char* get_macro_name_from_line(char* line);
int nth_word_at_line_equals(char* line, int index, char* word_to_compare);
Macro* register_new_macro_by_name(char* macro_name, MacroTable* macro_table);
Macro* register_line_to_macro(char* line, Macro* macro);
MacroTable* register_macro_to_table(Macro* macro, MacroTable* macro_table);
void skip_macro_def(AsmDescriptor* ds);

#endif
