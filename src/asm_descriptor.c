#include "../headers/asm_descriptor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/DataTable.h"
#include "../headers/constants.h"
#include "../headers/string_parsers.h"

AsmDescriptor *new_asm_descriptor(char *file_path) {
  AsmDescriptor *ds;
  char *fullName;
  char *rawName;
  fullName = cp_string(file_path);
  rawName =
      substring(file_path, 0, strlen(file_path) - strlen(ASM_FILE_SUFFIX));
  ds = (AsmDescriptor *)malloc(sizeof(AsmDescriptor));
  ds->fp = fopen(file_path, "r");
  ds->line = (char *)malloc(sizeof(char) * STRING_BUFFER_SIZE);
  ds->file_name = fullName;
  ds->line_num = 0;
  ds->line_num_str = itoa(0);
  ds->raw_file_name = rawName;
  ds->err_log = new_error_logger(stderr);
  ds->lable_tb = newLabelTable();
  ds->data_tb = newAsmTranslationTable();
  ds->instructions_tb = newAsmTranslationTable();
  return ds;
}

char *itoa(int num) {
  int i;
  int digit;
  int powers[3] = {100, 10, 1};
  char *result = cp_string("000");
  for (i = 0; i < 3; i++) {
    digit = num / powers[i];
    result[i] = '0' + digit;
    num = num % powers[i];
  }
  return result;
}

int get_next_line(AsmDescriptor *ds) {
  char *line;
  int was_successful = false;
  line = (char *)malloc(sizeof(char) * STRING_BUFFER_SIZE);
  line = fgets(line, STRING_BUFFER_SIZE, ds->fp);
  was_successful = line == NULL ? false : true;
  if (was_successful) {
    ds->line_num++;
    free(ds->line_num_str);
    ds->line_num_str = itoa(ds->line_num);
    strcpy(ds->line, line);
    free(line);
  }
  return !feof(ds->fp);
}

int is_asm_file(char *file_name) {
  char *suffix_start;
  int suffix_len;
  int file_name_len;
  file_name_len = strlen(file_name);
  suffix_len = strlen(ASM_FILE_SUFFIX);
  if (file_name_len < suffix_len)
    return false;
  suffix_start = file_name + (file_name_len - suffix_len - 1);
  return strcmp(suffix_start, ASM_FILE_SUFFIX) == 0;
}

char *get_file_name_without_suffix(AsmDescriptor *ds) {
  int file_name_len;
  int suffix_len;
  char *file_name_without_suffix;
  file_name_len = strlen(ds->file_name);
  suffix_len = strlen(ASM_FILE_SUFFIX);
  file_name_without_suffix = cp_string(ds->file_name);
  file_name_without_suffix[file_name_len - suffix_len] = '\0';
  return file_name_without_suffix;
}

void free_asm_descriptor(AsmDescriptor *ds) {
  if (ds == NULL)
    return;
  if (ds->line != NULL)
    free(ds->line);
  if (ds->fp != NULL)
    fclose(ds->fp);
  /* if (ds->line_num_str != NULL) */
  /* free(ds->line_num_str); */
  if (!ds->raw_file_name)
    free(ds->raw_file_name);
  if (ds->file_name != NULL)
    free(ds->file_name);
  clear_logger(ds->err_log);
  freeLabelTable(ds->lable_tb);
  freeAsmTranslationTable(ds->data_tb);
  freeAsmTranslationTable(ds->instructions_tb);
  free(ds);
}
