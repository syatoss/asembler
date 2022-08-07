#include <stdlib.h>
#include <string.h>

#include "../headers/asm_descriptor.h"
#include "../headers/constants.h"
#include "../headers/globals.h"
#include "../headers/line_paser.h"
#include "../headers/macro_regiester.h"
#include "../headers/macro_table.h"
#include "../headers/string_parsers.h"
#include "../headers/utils.h"

int is_macro_def(char *line) {
  return !is_comment_line(line) &&
         nth_word_at_line_equals(line, 1, MACRO_START);
}

int line_has_macro_def(char *line, MacroTable *table) {
  StrArr *words;
  int i;
  int lineHasMacro = false;
  if (is_comment_line(line))
    return false;
  words = get_line_words(line);
  for (i = 0; i < words->length; i++) {
    lineHasMacro =
        lineHasMacro || is_macro_in_table(get_str_at_index(words, i), table);
  }
  free_str_arr(words);
  return lineHasMacro;
}

Macro *register_new_macro_by_name(char *macro_name, MacroTable *macro_table) {
  if (is_macro_in_table(macro_name, macro_table)) {
    log_error(ds->err_log, "Duplicate macro definition");
    return NULL;
  }
  return get_new_macro(macro_name);
}

Macro *register_line_to_macro(char *line, Macro *macro) {
  add_str(macro->body, line);
  return macro;
}

MacroTable *register_macro_to_table(Macro *macro, MacroTable *macro_table) {
  add_macro_to_table(macro, macro_table);
  return macro_table;
}

int is_macro_def_end(char *line) {
  return !is_comment_line(line) && nth_word_at_line_equals(line, 1, MACRO_END);
}

void skip_macro_def(AsmDescriptor *ds) {
  while (!is_macro_def_end(ds->line))
    get_next_line(ds);
}

char *get_macro_name_from_line(char *line) {
  char *name;
  char *name_cp;
  StrArr *words = get_line_words(line);
  name = get_str_at_index(words, MACRO_NAME_WORD_INDEX);
  name_cp = (char *)malloc(sizeof(char) * (strlen(name) + 1));
  if (!name_cp)
    SYS_MEM_FAIL_EXIT(1);
  name_cp = strcpy(name_cp, name);
  free_str_arr(words);
  return name_cp;
}

int nth_word_at_line_equals(char *line, int n, char *word_to_compare) {
  StrArr *words;
  int equals = false;
  words = get_line_words(line);
  if (words->length >= 1)
    equals = strcmp(get_str_at_index(words, n - 1), word_to_compare) == 0;
  free_str_arr(words);
  return equals;
}
