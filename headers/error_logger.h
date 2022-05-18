#include "./str_table.h"

typedef struct {
    int has_errors;
    int error_count;
    str_table* errors;
} error_logger;

error_logger* new_error_logger(FILE* );
void log_error(error_logger* , char* );
void clear_logger(error_logger* );
void print_all_logger_errors(error_logger* );
