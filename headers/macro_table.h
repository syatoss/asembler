#include "./str_table.h"

typedef struct {
    char* name;
    StrTable* body;
} Macro;

typedef struct macroNode {
    Macro* macro;
    struct macroNode* next;
} MacroNode;

typedef struct {
    MacroNode* head;
} MacroList;

typedef struct {
    MacroList* macros;
    int macro_count;
} MacroTable;

MacroNode* get_new_macro_node(Macro* );
void add_macro_to_list(Macro* , MacroList* );
Macro* get_new_macro(char* );
void free_macro(Macro* );
MacroNode* get_macro_node_by_name_from_list(char* , MacroList* );
int is_macro_in_table(char* , MacroTable* );
MacroList* init_macro_list();
MacroTable* init_macro_table();
void free_macro_node(MacroNode*);
void free_macro_list(MacroList* );
void clear_macro_table(MacroTable* );
