#include "../headers/asm_descriptor.h"

#include <stdlib.h>
#include <string.h>

#include "../headers/DataTable.h"
#include "../headers/constants.h"
#include "../headers/string_parsers.h"

AsmDescriptor *new_asm_descriptor(char *raw_file_name, char *file_extennsion) {
  AsmDescriptor *ds;
  char *err;
  char *file_path;
  file_path = cat_strings(raw_file_name, file_extennsion, NULL);
  ds = (AsmDescriptor *)malloc(sizeof(AsmDescriptor));
  ds->fp = fopen(file_path, "r");
  ds->line = (char *)malloc(sizeof(char) * STRING_BUFFER_SIZE);
  ds->file_name = file_path;
  ds->line_num = 0;
  ds->raw_file_name = cp_string(raw_file_name);
  ds->line_num_string = itoa(ds->line_num, 10);
  ds->err_log = new_error_logger(stderr);
  ds->lable_tb = newLabelTable();
  ds->data_tb = newAsmTranslationTable();
  ds->instructions_tb = newAsmTranslationTable();
  if (!ds->fp) {
    err = cat_strings("Error, cant find ", ds->file_name, NULL);
    log_error(ds->err_log, err);
    free(err);
  }
  return ds;
}

int get_next_line(AsmDescriptor *ds) {
  char *line;
  int was_successful = false;
  line = (char *)malloc(sizeof(char) * STRING_BUFFER_SIZE);
  line = fgets(line, STRING_BUFFER_SIZE, ds->fp);
  was_successful = line == NULL ? false : true;
  if (was_successful) {
    ds->line_num++;
    free(ds->line_num_string);
    ds->line_num_string = itoa(ds->line_num, 10);
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
  if (ds->err_log != NULL)
    clear_logger(ds->err_log);
  if (ds->line_num_string != NULL) {
    free(ds->line_num_string);
  }
  if (ds->file_name) {
    free(ds->file_name);
  }
  if (ds->raw_file_name) {
    free(ds->raw_file_name);
  }
  freeLabelTable(ds->lable_tb);
  freeAsmTranslationTable(ds->data_tb);
  freeAsmTranslationTable(ds->instructions_tb);
  free(ds);
}
