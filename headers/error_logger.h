#include "./str_table.h"

typedef struct {
    int has_errors;
    int error_count;
    StrTable* errors;
} ErrorLogger;

ErrorLogger* new_error_logger(FILE* );
void log_error(ErrorLogger* , char* );
void clear_logger(ErrorLogger* );
void print_all_logger_errors(ErrorLogger*);
