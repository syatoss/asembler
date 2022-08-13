#ifndef PRE_PROC_H
#define PRE_PROC_H

#include <stdio.h>

#include "./asm_descriptor.h"
#include "./macro_table.h"

/* creates the spread file for the current file registered in AsmDescriptor */
void generate_spread_file();
/* registers all the macros in the current AsmDescriptor to macro_table */
void scan_macros(MacroTable *macro_table);
/* reads the body of a macro from the file assosiated to ds and registers it in
 * macro */
Macro *read_macro_body(Macro *macro, AsmDescriptor *ds);
/* spreads all macros from macro_table to targets file in each macro's name
 * aprearance in the current handled file (hosted in the global AsmDescriptor)
 */
void spread_macros_to_a_file(MacroTable *macro_table, FILE *target);
/* spreads all macros from macro_table to targets file in each macro's name
 * aprearance in the current handled file (hosted in the global AsmDescriptor)
 */
void write_spread_to_file(FILE *target, MacroTable *macro_table);
/* appends the macro identified by macro_name from macro_table to target */
void append_macro_to_file(char *macro_name, MacroTable *macro_table,
                          FILE *target);

#endif
