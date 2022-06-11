#ifndef ASM_DESCRIPTOR_H
#define ASM_DESCRIPTOR_H

#include <stdio.h>

#include "./error_logger.h"
#include "./utils.h"

typedef struct {
    FILE* fp;
    char* file_name;
    char* line;
    int line_num;
    ErrorLogger* err_log;
    /* LableTable* lable_tb; */
    /* CodeTable* code_tb; */

} AsmDescriptor;

AsmDescriptor* new_asm_descriptor(char*);
int get_next_line(AsmDescriptor*);
void free_asm_descriptor(AsmDescriptor*);
char* get_file_name_without_suffix(AsmDescriptor* ds);
int is_asm_file(char* file_name);

#endif
