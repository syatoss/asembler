#include <stdlib.h>
#include "../headers/str_table.h"

str_table* new_str_table(FILE* fd) {
    str_table* table = malloc(sizeof(str_table));
    table->num_of_lines = 0;
    table->strings = new_str_linked_list();
    table->print_target = fd;
    return table;
}

void add_str(str_table* tb,char* str) {
    str_node* node = get_new_node(str);
    add_node(tb->strings, node);
}

void println_str(str_table* tb,char* str) {
    fprintf(tb->print_target, "%s\n", str);
}

void print_table(str_table* tb) {
    str_node* head = tb->strings->head;
    while(head) {
        println_str(tb, head->string);
        head = get_next_node(head);
    }
}


void clear_str_table(str_table* tb) {
    if(!tb) return;
    clear_linked_list(tb->strings);
    free(tb);
}


