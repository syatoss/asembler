#include "./str_table.h"

typedef struct {
    int has_errors;
    int error_count;
    str_table* errors;
} error_logger;
