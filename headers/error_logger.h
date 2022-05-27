#ifndef SRT_TABLE
#define SRT_TABLE
#include "./str_table.h"
#endif

typedef struct {
    int has_errors;
    int error_count;
    StrTable* errors;
} ErrorLogger;

ErrorLogger* new_error_logger(FILE*);
void log_error(ErrorLogger*, char*);
void clear_logger(ErrorLogger*);
void print_all_logger_errors(ErrorLogger*);
