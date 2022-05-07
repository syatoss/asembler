#include <stdio.h>
#include <stdlib.h>
#include "../headers/utils.h"
#include "../headers/error_logger.h"
#include "../headers/constants.h"

error_logger* new_error_logger(FILE* print_target) {
    error_logger* el;
    str_table* err_tb;
    el = malloc(sizeof(error_logger));
    if(!el) SYS_MEM_FAIL_EXIT(1)
    err_tb = new_str_table(print_target);
    el->error_count = 0;
    el->has_errors = false;
    el->errors = err_tb;
    return el;
}

void log_error(error_logger* logger, char* error) {

}

