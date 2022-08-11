#include "../headers/main.h"

#include <stdio.h>
#include <stdlib.h>

#include "../headers/LabelTable.h"
#include "../headers/Translation.h"
#include "../headers/asm_descriptor.h"
#include "../headers/constants.h"
#include "../headers/firstScan2.h"
#include "../headers/label.h"
#include "../headers/macro_regiester.h"
#include "../headers/secondScan.h"
#include "../headers/string_parsers.h"

AsmDescriptor *ds;

void for_each_cmd_param(char *cmd_param[], const int parm_count,
                        void callback(char *)) {
  int i;
  for (i = 1; i < parm_count; i++) {
    callback(cmd_param[i]);
  }
}

void run_pre_proc(char *spread_file_name) {
  generate_spread_file(spread_file_name);
  if (ds->err_log->has_errors)
    print_all_logger_errors(ds->err_log);
}

void assembler(char *asm_file_name) {
  char *spread_file_name = cat_strings(asm_file_name, SPREAD_FILE_SUFFIX, NULL);

  /* attemt to open file */
  ds = new_asm_descriptor(asm_file_name, ASM_FILE_SUFFIX);
  if (ds->err_log->has_errors) {
    print_all_logger_errors(ds->err_log);
    free_asm_descriptor(ds);
    free(spread_file_name);
    exit(1);
  }

  /* attempt to run the pre processor */
  run_pre_proc(spread_file_name);
  if (ds->err_log->has_errors) {
    print_all_logger_errors(ds->err_log);
    free_asm_descriptor(ds);
    free(spread_file_name);
    exit(1);
  }
  free_asm_descriptor(ds);

  /* attempt to run the first scan */
  ds = new_asm_descriptor(asm_file_name, SPREAD_FILE_SUFFIX);
  firstScan(ds);
  if (ds->err_log->has_errors) {
    print_all_logger_errors(ds->err_log);
    free_asm_descriptor(ds);
    free(spread_file_name);
    exit(1);
  }

  /* run the second scan if no errors found */
  secondScan();
  free_asm_descriptor(ds);
  free(spread_file_name);
}

int main(int argc, char *argv[]) {
  for_each_cmd_param(argv, argc, assembler);
  return 0;
}
