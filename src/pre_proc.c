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

extern AsmDescriptor* ds;

void generate_spread_file(char* spread_file_name) {
    MacroTable* macro_table = init_macro_table();
    char* err = NULL;
    FILE* spread_file = fopen(spread_file_name, "w");
    if (!spread_file) {
        err = cat_strings("Error in file ", ds->file_name,
                          ", not a valid assembly file.", NULL);
        log_error(ds->err_log, err);
        free(err);
    }
    scan_macros(macro_table);
    if (!ds->err_log->has_errors)
        spread_macros_to_a_file(macro_table, spread_file);
    fclose(spread_file);
}

void spread_macros_to_a_file(MacroTable* macro_table, FILE* target) {
    /* goes to the beggining of the file */
    fseek(ds->fp, 0, SEEK_SET);
    ds->line_num = 0;
    write_spread_to_file(target, macro_table);
}

void write_spread_to_file(FILE* target, MacroTable* macro_table) {
    while (!feof(ds->fp) && get_next_line(ds)) {
        if (is_comment_line(ds->line)) continue;
        if (is_macro_def(ds->line)) {
            skip_macro_def(ds);
            continue;
        }
        // needs to replace macro if is not taking entire line
        if (line_has_macro_def(ds->line, macro_table)) {
            append_macro_to_file(ds->line, macro_table, target);
            continue;
        }
        fprintf(target, "%s\n", ds->line);
    }
}

void append_macro_to_file(char* line, MacroTable* macro_table, FILE* target) {
    StrArr* words;
    int i;
    Macro* macro;
    words = get_line_words(line);
    for (i = 0; i < words->length; i++) {
        macro = get_macro_by_name_from_table(get_str_at_index(words, i),
                                             macro_table);
        macro ? print_macro(macro, target)
              : fprintf(target, "%s", get_str_at_index(words, i));
    }
    /* Macro* macro = get_macro_by_name_from_table(macro_name, macro_table); */
    /* if (!macro) return; */
    /* print_macro(macro, target); */
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
        if (macro) register_line_to_macro(ds->line, macro);
    }
    return macro;
}
