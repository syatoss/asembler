#ifndef STRING_PARSERS_H
#define STRING_PARSERS_H

typedef struct {
  int length;
  char **strings;
} StrArr;

/* removes white space form the right end of the string */
char *rtrim(char *);
int is_white_space(char);
/* removes white space form the left end of the string */
char *ltrim(char *);
/* removes white space form both ends of the string */
char *trim(char *s);
/* returns a new allocated substring of target, starting at index start and
 * spaning n chars */
char *substring(char *source, int start, int n);
/* splits string by the delimiter to multiple strings save in new StrArr,
 * pointer to the newly allocated StrArr is returned */
StrArr *split(char *string, char *delimiter);
/* allocates new StrArr, assinged defaults and returns a pointer to the
 * allocated memmory */
StrArr *new_str_arr();
/* returns the string at index */
char *get_str_at_index(StrArr *array, int index);
/* returns newly allocated StrArr holding the words in a line (words that were
 * prevoiusly sepereated by space) */
StrArr *get_line_words(char *line);
/* frees the pointer and all of its assosiated memmory */
void free_str_arr(StrArr *arr);
/* returns a pointer to newly alocated memmory containing the copied string */
char *cp_string(char *str);
/* appends str_to_append to target, returns newlly allocated string that is the
 * appended string */
char *cat_string(char *target, char *str_to_append);
/* concatenetes multiple strings to target returns pointer to
target's newly alocated memmory takes NULL as a flag indicating last
  string  */
char *cat_strings(char *target, ...);
/* function for debugging */
void printStrArr(StrArr *arr);
/* convers num to ints string form in the provided base, returns pointer to the
 * newly allocated string */
char *itoa(int num, int base);
/* reverses a string witout new memmory allocation */
void reverseInPlace(char *s);

#endif
