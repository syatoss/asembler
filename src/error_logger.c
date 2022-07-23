#include <stdio.h>
#include <stdlib.h>
#include "../headers/utils.h"
#include "../headers/error_logger.h"
#include "../headers/constants.h"

ErrorLogger* new_error_logger(FILE* print_target) {
    ErrorLogger* el;
    StrTable* err_tb;
    el = (ErrorLogger*)malloc(sizeof(ErrorLogger));
    if(!el) SYS_MEM_FAIL_EXIT(1);
    err_tb = new_str_table(print_target);
    el->error_count = 0;
    el->has_errors = false;
    el->errors = err_tb;
    return el;
}


void log_error(ErrorLogger* logger, char* error) {
    add_str(logger->errors, error);
    logger->has_errors = true;
    logger->error_count++;
}

void clear_logger(ErrorLogger* logger) {
    clear_str_table(logger->errors);
    free(logger);
}

void print_all_logger_errors(ErrorLogger* logger) {
    print_table(logger->errors, stderr);
}

