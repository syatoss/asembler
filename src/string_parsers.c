#include "./../headers/string_parsers.h"

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "./../headers/utils.h"

char *ltrim(char *s) {
  while (is_white_space(*s))
    s++;
  return s;
}

StrArr *new_str_arr() {
  StrArr *arr;
  arr = (StrArr *)malloc(sizeof(StrArr));
  if (!arr)
    SYS_MEM_FAIL_EXIT(1);
  arr->strings = NULL;
  arr->length = 0;
  return arr;
}

char *cat_strings(char *target, ...) {
  va_list string_to_concat;
  char *concatenated = NULL;
  char *next_string;
  va_start(string_to_concat, target);
  concatenated = cp_string(target);
  while ((next_string = va_arg(string_to_concat, char *)))
    concatenated = cat_string(concatenated, next_string);
  va_end(string_to_concat);
  return concatenated;
}

char *cat_string(char *target, char *str_to_append) {
  char *appended = cp_string(target);
  appended = (char *)realloc(
      appended, sizeof(char) * (strlen(appended) + strlen(str_to_append) + 1));
  strcat(appended, str_to_append);
  free(target);
  return appended;
}

void reverseInPlace(char *s) {
  int first = 0;
  char pivot;
  int last;
  last = strlen(s);
  while (first < last) {
    pivot = s[first];
    s[first] = s[last];
    s[last] = pivot;
    first++;
    last++;
  }
}

char *itoa(int num, int base) {
  char *stringNum = NULL;
  int digits = 0;
  int numCopy = num;
  int i;
  int currntDigit;
  num = 22;
  printf("start of itoa (num=%d, base=%d): \n", num, base);
  do {
    numCopy = numCopy / base;
    digits++;
  } while (numCopy);
  stringNum = (char *)malloc(sizeof(char) * (digits + 1));
  for (i = 0; i < digits; i++) {
    currntDigit = (num % ((int)(pow(base, digits - 1 - i))));
    printf("digit of the %d iteration: %d\n", i, currntDigit);
    stringNum[i] = '0' + currntDigit;
    num = num / ((int)(pow(base, digits - 1 - i)));
  }
  stringNum[i] = '\0';
  printf("end of itoa: \n\n\n");
  return stringNum;
}

void free_str_arr(StrArr *arr) {
  int i = 0;
  while (i < arr->length)
    free(arr->strings[i++]);
  free(arr->strings);
  free(arr);
}

char *cp_string(char *str) {
  char *str_cp;
  int new;
  new = str == NULL;
  str_cp = !new ? (char *)malloc(sizeof(char) * (strlen(str) + 1))
                : (char *)malloc(sizeof(char));
  /* str_cp = (char *)realloc(str_cp, sizeof(char) * (strlen(str) + 1)); */
  !new ? strcpy(str_cp, str) : strcpy(str_cp, "");
  return str_cp;
}

void push_next_string(StrArr *array, char *str) {
  char *str_cp;
  char **new_string_local;
  str_cp = (char *)malloc(sizeof(char) * (strlen(str) + 1));
  if (!str_cp)
    SYS_MEM_FAIL_EXIT(1);
  new_string_local =
      array->strings != NULL
          ? (char **)realloc(array->strings,
                             sizeof(char *) * (array->length + 1))
          : (char **)malloc(sizeof(char *) * (array->length + 1));
  array->strings = new_string_local;
  str_cp = strcpy(str_cp, str);
  array->strings[array->length] = str_cp;
  array->length++;
}

StrArr *get_line_words(char *line) { return split(line, " "); }

char *get_str_at_index(StrArr *array, int index) {
  if (index >= array->length) {
    fprintf(stderr,
            "Invalid use of function \"get_str_at_index\" index exceeds "
            "the array length");
    exit(2);
  }
  return array->strings[index];
}

StrArr *split(char *str, char *delimiter) {
  StrArr *arr;
  char *token;
  char *pivot = NULL;
  char *str_cp = NULL;
  str_cp = trim(str);
  arr = new_str_arr();
  token = strtok(str_cp, delimiter);
  while (token != NULL) {
    pivot = trim(token);
    push_next_string(arr, token);
    token = strtok(NULL, delimiter);
    free(pivot);
    pivot = NULL;
  }
  if (pivot != NULL)
    free(pivot);
  free(str_cp);
  return arr;
}

int is_white_space(char c) { return c == ' ' || c == '\t' || c == '\n'; }

char *rtrim(char *s) {
  int original_len = strlen(s);
  char last_char = *(s + original_len);
  char *back = s + original_len;
  while (is_white_space(*(--back)))
    ;
  if (last_char == '\n')
    *(++back) = last_char;
  back++;
  *(back) = '\0';
  return s;
}

void printStrArr(StrArr *arr) {
  int i;
  for (i = 0; i < arr->length; i++) {
    printf("arr[%d]: %s\n", i, arr->strings[i]);
  }
}

char *trim(char *s) {
  char *trimmed;
  char *pivot;
  trimmed = cp_string(s);
  pivot = trimmed;
  trimmed = rtrim(ltrim(s));
  trimmed = cp_string(trimmed);
  free(pivot);
  return trimmed;
}

char *substring(char *target, const char *source, int start, int n) {
  while (n > 0) {
    *target = *(source + start);
    target++;
    source++;
    n--;
  }
  *target = '\0';
  return target;
}
