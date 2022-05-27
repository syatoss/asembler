#include <stdio.h>

#ifndef ERROR_LOGGER
#define ERROR_LOGGER
#include "./error_logger.h"
#endif

#include "./utils.h"

typedef struct {
    FILE* fp;
    char* file_name;
    char* line;
    int line_num;
    ErrorLogger* err_log;
} AsmDescriptor;

AsmDescriptor* new_asm_descriptor(char*);
int get_next_line(AsmDescriptor*);
void free_asm_descriptor(AsmDescriptor*);

