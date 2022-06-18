#include "../headers/main.h"

#include <stdio.h>
#include <stdlib.h>

#include "../headers/asm_descriptor.h"
#include "../headers/constants.h"
#include "../headers/macro_regiester.h"
#include "../headers/string_parsers.h"

AsmDescriptor* ds;

void for_each_cmd_param(char* cmd_param[], const int parm_count,
                        void callback(char*)) {
    int i;
    for (i = 1; i < parm_count; i++) {
        callback(cmd_param[i]);
    }
}

void run_pre_proc(char* asm_file_name) {
    char* spread_file_name = cp_string(asm_file_name);
    char* full_file_name = cp_string(asm_file_name);
    spread_file_name = cat_string(spread_file_name, SPREAD_FILE_SUFFIX);
    full_file_name = cat_string(full_file_name, ASM_FILE_SUFFIX);
    ds = new_asm_descriptor(full_file_name);
    generate_spread_file(spread_file_name);
    if (ds->err_log->has_errors) print_all_logger_errors(ds->err_log);
    free(spread_file_name);
    free(full_file_name);
    free_asm_descriptor(ds);
}

int main(int argc, char* argv[]) {
    for_each_cmd_param(argv, argc, run_pre_proc);
    /* int i; */
    /* int was_next_line_read; */
    /* char* macroName; */
    /* for (i = 1; i < argc; i++) { */
    /*     ds = new_asm_descriptor(argv[i]); */
    /*     while (!feof(ds->fp)) { */
    /*         was_next_line_read = get_next_line(ds); */
    /*         if (was_next_line_read && is_macro_def(ds->line)) { */
    /*             macroName = get_macro_name_from_line(ds->line); */
    /*             printf("Macro name: %s\n", macroName); */
    /*             free(macroName); */
    /*         } */
    /*     } */
    /*     free_asm_descriptor(ds); */
    /* } */
    return 0;
}
