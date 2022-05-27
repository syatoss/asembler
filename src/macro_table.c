#include "../headers/macro_table.h"

#include <stdlib.h>
#include <string.h>

#include "../headers/utils.h"

MacroTable* init_macro_table() {
    MacroTable* macroTable = (MacroTable*)malloc(sizeof(MacroTable));
    if (!macroTable) SYS_MEM_FAIL_EXIT(1);
    macroTable->macro_count = 0;
    macroTable->macros = init_macro_list();
    return macroTable;
}

void clear_macro_table(MacroTable* macroTable) {
    free_macro_list(macroTable->macros);
    free(macroTable);
}

void free_macro_list(MacroList* macroList) {
    MacroNode* node_to_free = macroList->head;
    while (macroList->head) {
        macroList->head = macroList->head->next;
        free_macro_node(node_to_free);
    }
    free(macroList);
}

void free_macro_node(MacroNode* macroNode) {
    free_macro(macroNode->macro);
    free(macroNode);
}

MacroList* init_macro_list() {
    MacroList* macroList = (MacroList*)malloc(sizeof(MacroList));
    if (!macroList) SYS_MEM_FAIL_EXIT(1);
    macroList->head = NULL;
    return macroList;
}

MacroList* get_new_macro_list() {
    MacroList* macroList = (MacroList*)malloc(sizeof(MacroList));
    if (!macroList) SYS_MEM_FAIL_EXIT(1);
    macroList->head = NULL;
    return macroList;
}

void add_macro_to_table(Macro* macro, MacroTable* macroTable) {
    add_macro_to_list(macro, macroTable->macros);
    macroTable->macro_count++;
}

void add_macro_to_list(Macro* macro, MacroList* macroList) {
    MacroNode* macroNode = get_new_macro_node(macro);
    macroNode->next = macroList->head;
    macroList->head = macroNode;
}

Macro* get_macro_by_name_from_table(char* macroName, MacroTable* macroTable) {
    MacroNode* macroNode =
        get_macro_node_by_name_from_list(macroName, macroTable->macros);
    if (!macroNode) return NULL;
    return macroNode->macro;
}

int is_macro_in_table(char* macroName, MacroTable* macroTable) {
    return get_macro_by_name_from_table(macroName, macroTable) != NULL;
}

MacroNode* get_macro_node_by_name_from_list(char* macroName,
                                            MacroList* macroList) {
    MacroNode* currentMacro = macroList->head;
    while (currentMacro && !strcmp(macroName, currentMacro->macro->name))
        currentMacro = currentMacro->next;
    return currentMacro;
}

MacroNode* get_new_macro_node(Macro* macro) {
    MacroNode* macroNode = (MacroNode*)malloc(sizeof(MacroNode));
    if (!macroNode) SYS_MEM_FAIL_EXIT(1);
    macroNode->macro = macro;
    macroNode->next = NULL;
    return macroNode;
}

void print_macro(Macro* macro, FILE* print_target) {
    print_table(macro->body, print_target);
}

Macro* get_new_macro(char* name) {
    Macro* macro = (Macro*)malloc(sizeof(Macro));
    if (!macro) SYS_MEM_FAIL_EXIT(1);
    macro->name = (char*)malloc(sizeof(char) * (strlen(name)) + 1);
    macro->name = strcpy(macro->name, name);
    macro->body = new_str_table();
    return macro;
}

void free_macro(Macro* macro) {
    free(macro->name);
    clear_str_table(macro->body);
}
