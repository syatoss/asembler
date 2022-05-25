#define MACRO_NAME_WORD_INDEX 1

int is_macro_def(char* );
int is_macro_def_end(char* );
char* get_macro_name_from_line(char* line);
int nth_word_at_line_equals(char* line,int index, char* word_to_compare);
