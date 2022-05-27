#ifndef PRE_PROC_H
#define PRE_PROC_H

#include <stdio.h>

#include "./asm_descriptor.h"
#include "./macro_table.h"
#include "./services.h"

void generate_spread_file();
void scan_macros(MacroTable* macro_table);
Macro* read_macro_body(Macro* macro, AsmDescriptor* ds);
void spread_macros_to_a_file(MacroTable* macro_table);
void write_spread_to_file(FILE* target, MacroTable* macro_table);
void append_macro_to_file(char* macro_name, MacroTable* macro_table,
                          FILE* target);

#endif
