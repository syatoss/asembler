#include "../headers/pre_proc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/asm_descriptor.h"
#include "../headers/constants.h"
#include "../headers/globals.h"
#include "../headers/line_paser.h"
#include "../headers/macro_regiester.h"
#include "../headers/macro_table.h"
#include "../headers/master_header.h"
#include "../headers/string_parsers.h"
#include "../headers/utils.h"

void generate_spread_file() {
    MacroTable* macro_table = init_macro_table();
    scan_macros(macro_table);
    if (ds->err_log->has_errors) {
        print_all_logger_errors(ds->err_log);
        return;
    }
}

void spread_macros_to_a_file(MacroTable* macro_table) {
    char* spreaded_file_name;
    FILE* spread_file;
    spreaded_file_name =
        cat_string(get_file_name_without_suffix(ds), SPREAD_FILE_SUFFIX);
    spread_file = fopen(spreaded_file_name, "w");
    if (!spread_file) SYS_MEM_FAIL_EXIT(1);
    fseek(ds->fp, 0, SEEK_SET);  // go to the beggining of the file
    write_spread_to_file(spread_file, macro_table);
    free(spreaded_file_name);
    fclose(spread_file);
}

void write_spread_to_file(FILE* target, MacroTable* macro_table) {
    while (!feof(ds->fp)) {
        if (is_comment_line(ds->line)) continue;
        if (is_macro_def(ds->line)) skip_macro_def(ds);
        if (is_macro_in_table(trim(ds->line), macro_table)) {
            append_macro_to_file(trim(ds->line), macro_table, target);
            continue;
        }
        fprintf(target, "%s", ds->line);
    }
}

void append_macro_to_file(char* macro_name, MacroTable* macro_table,
                          FILE* target) {
    Macro* macro = get_macro_by_name_from_table(macro_name, macro_table);
    if (!macro) return;
    print_macro(macro, target);
}

void scan_macros(MacroTable* macro_table) {
    char* macro_name;
    Macro* current_macro;
    while (!feof(ds->fp)) {
        if (!get_next_line(ds) || !is_macro_def(ds->line)) continue;
        macro_name = get_macro_name_from_line(ds->line);
        current_macro = register_new_macro_by_name(macro_name, macro_table);
        current_macro = read_macro_body(current_macro, ds);
        add_macro_to_table(current_macro, macro_table);
        free(macro_name);
    }
}

Macro* read_macro_body(Macro* macro, AsmDescriptor* ds) {
    while (!feof(ds->fp)) {
        if (!get_next_line(ds)) continue;
        if (is_macro_def_end(ds->line)) return macro;
        register_line_to_macro(ds->line, macro);
    }
    return macro;
}
