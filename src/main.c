#include "../headers/main.h"

#include <stdio.h>
#include <stdlib.h>

#include "../headers/LabelTable.h"
#include "../headers/Translation.h"
#include "../headers/asm_descriptor.h"
#include "../headers/constants.h"
#include "../headers/firstscan.h"
#include "../headers/label.h"
#include "../headers/macro_regiester.h"
#include "../headers/secondScan.h"
#include "../headers/string_parsers.h"

AsmDescriptor *ds;

void runSecondScan(char *full_file_name) {
  full_file_name = cat_strings(NULL, full_file_name, ASM_FILE_SUFFIX, NULL);
  ds = new_asm_descriptor(full_file_name);
  secondScan();
}

void for_each_cmd_param(char *cmd_param[], const int parm_count,
                        void callback(char *)) {
  int i;
  for (i = 1; i < parm_count; i++) {
    callback(cmd_param[i]);
  }
}

void run_pre_proc(char *spread_file_name) {
  /* char *spread_file_name = cp_string(asm_file_name); */
  /* char *full_file_name = cp_string(asm_file_name); */
  /* spread_file_name = cat_string(spread_file_name, SPREAD_FILE_SUFFIX); */
  /* full_file_name = cat_string(full_file_name, ASM_FILE_SUFFIX); */
  /* ds = new_asm_descriptor(full_file_name); */
  generate_spread_file(spread_file_name);
  if (ds->err_log->has_errors)
    print_all_logger_errors(ds->err_log);
  /* free(spread_file_name); */
  /* free(full_file_name); */
}

void assembler(char *asm_file_name) {
  char *spread_file_name =
      cat_strings(NULL, asm_file_name, SPREAD_FILE_SUFFIX, NULL);
  char *full_file_name =
      cat_strings(NULL, asm_file_name, ASM_FILE_SUFFIX, NULL);
  ds = new_asm_descriptor(full_file_name);
  run_pre_proc(spread_file_name);
  if (ds->err_log->has_errors) {
    print_all_logger_errors(ds->err_log);
    free_asm_descriptor(ds);
    free(spread_file_name);
    free(full_file_name);
    exit(1);
  }
  free_asm_descriptor(ds);
  ds = new_asm_descriptor(spread_file_name);
  firstscan();
  if (ds->err_log->has_errors) {
    print_all_logger_errors(ds->err_log);
    free_asm_descriptor(ds);
    free(spread_file_name);
    free(full_file_name);
    exit(1);
  }
  secondScan();
  free_asm_descriptor(ds);
  free(spread_file_name);
  free(full_file_name);
}

int main(int argc, char *argv[]) {
  for_each_cmd_param(argv, argc, assembler);
  return 0;
}
