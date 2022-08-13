#include "../headers/str_table.h"
#include "../headers/utils.h"
#include <stdlib.h>

StrTable *new_str_table(FILE *print_target) {
  StrTable *table = (StrTable *)malloc(sizeof(StrTable));
  if (!table)
    SYS_MEM_FAIL_EXIT(1)
  table->num_of_lines = 0;
  table->strings = new_str_linked_list();
  return table;
}

void add_str(StrTable *tb, char *str) {
  StrNode *node;
  node = get_new_node(str);
  add_node(tb->strings, node);
}

void println_str(StrTable *tb, char *str, FILE *print_target) {
  fprintf(print_target, "%s\n", str);
}

void print_table(StrTable *tb, FILE *print_target) {
  StrNode *head = tb->strings->head;
  while (head) {
    println_str(tb, head->string, print_target);
    head = get_next_node(head);
  }
}

void clear_str_table(StrTable *tb) {
  if (!tb)
    return;
  clear_linked_list(tb->strings);
  free(tb);
}
