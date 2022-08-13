#include "../headers/assembler.h"

#include <stdio.h>
#include <stdlib.h>

#include "../headers/LabelTable.h"
#include "../headers/Translation.h"
#include "../headers/asm_descriptor.h"
#include "../headers/constants.h"
#include "../headers/firstScan.h"
#include "../headers/label.h"
#include "../headers/macro_regiester.h"
#include "../headers/secondScan.h"
#include "../headers/string_parsers.h"

/*
 * ASSUMPTIONS
 *
 * ass described in the project's definition it is assumed that all macro defs
 * have macroend line this is not checked, it is assumed that translated code
 * does not exeed 155 lines it is assumed that a line of assembly is not
 * exeeding the allowed length it is assumed that strings defined by .string and
 * .struct are only containit alphabetic characters upper case and lower case
 * (this is being specifically checked)
 * */

/*
 * The program executes 3 seperate steps,
 * pre-processor, first scan and second scan
 * for each step the assembler function stops to check if any errors
 * were accumilated if so the run of the program stops executing and writes the
 * errors to strerr if not the program continues to the next step. the
 * information about the state of the program (which line is read the errors
 * that were accumilated and the tables of data and insturctions are saved in an
 * object named AsmblerDescriptor this object is global)
 * */

AsmDescriptor *ds;
void assembler(char *asm_file_name);
void run_pre_proc(char *spread_file_name);
void for_each_cmd_param(char *cmd_param[], const int parm_count,
                        void callback(char *));

void for_each_cmd_param(char *cmd_param[], const int parm_count,
                        void callback(char *)) {
  int i;
  for (i = 1; i < parm_count; i++) {
    callback(cmd_param[i]);
  }
}

void run_pre_proc(char *spread_file_name) {
  generate_spread_file(spread_file_name);
}

void assembler(char *asm_file_name) {
  char *spread_file_name = cat_strings(asm_file_name, SPREAD_FILE_SUFFIX, NULL);

  /* attemt to open file */
  ds = new_asm_descriptor(asm_file_name, ASM_FILE_SUFFIX);
  if (ds->err_log->has_errors) {
    print_all_logger_errors(ds->err_log);
    free_asm_descriptor(ds);
    free(spread_file_name);
    return;
  }

  /* attempt to run the pre processor */
  run_pre_proc(spread_file_name);
  if (ds->err_log->has_errors) {
    print_all_logger_errors(ds->err_log);
    free_asm_descriptor(ds);
    free(spread_file_name);
    return;
  }
  free_asm_descriptor(ds);

  /* attempt to run the first scan */
  ds = new_asm_descriptor(asm_file_name, SPREAD_FILE_SUFFIX);
  firstScan(ds);
  if (ds->err_log->has_errors) {
    print_all_logger_errors(ds->err_log);
    free_asm_descriptor(ds);
    free(spread_file_name);
    return;
  }

  /* run the second scan if no errors found */
  rewind(ds->fp);
  secondScan();
  free_asm_descriptor(ds);
  free(spread_file_name);
}

int main(int argc, char *argv[]) {
  for_each_cmd_param(argv, argc, assembler);
  return 0;
}
