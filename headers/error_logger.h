#ifndef ERROR_LOGGER_H
#define ERROR_LOGGER_H

#include "./str_table.h"

typedef struct {
  int has_errors;
  int error_count;
  StrTable *errors;
} ErrorLogger;

/* allocates new memory for error logger and initates default values */
ErrorLogger *new_error_logger(FILE *);
void log_error(ErrorLogger *, char *);
/* frees all allocated memory assosiated with the error logger */
void clear_logger(ErrorLogger *);
/* prints the errors to stderr */
void print_all_logger_errors(ErrorLogger *);

#endif
