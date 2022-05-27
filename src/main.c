#include "../headers/main.h"

#include <stdio.h>
#include <stdlib.h>

#include "../headers/asm_descriptor.h"
#include "../headers/macro_regiester.h"

AsmDescriptor* ds;

int main(int argc, char* argv[]) {
    int i;
    int was_next_line_read;
    char* macroName;
    for (i = 1; i < argc; i++) {
        ds = new_asm_descriptor(argv[i]);
        while (!feof(ds->fp)) {
            was_next_line_read = get_next_line(ds);
            if (was_next_line_read && is_macro_def(ds->line)) {
                macroName = get_macro_name_from_line(ds->line);
                printf("Macro name: %s\n", macroName);
                free(macroName);
            }
        }
        free_asm_descriptor(ds);
    }
    return 0;
}
