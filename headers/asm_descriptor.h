#ifndef ASM_DESCRIPTOR_H
#define ASM_DESCRIPTOR_H

#include <stdio.h>
#include "./LabelTable.h"

#include "./error_logger.h"
#include "./utils.h"

typedef struct {
    FILE* fp;
    char* file_name;
    char* line;
    int IC;
    int DC;
    int line_num;
    ErrorLogger* err_log;
    LabelTable* labelTable;
//    CodeTable* code_tb;

} AsmDescriptor;

/* creates a new strunt that holds all the information needed for the assembler
 * to run */
AsmDescriptor* new_asm_descriptor(char*);
/* reads the next line from the file in the AsmDescriptor and assignes it to the
 * line field in the variable, trurn true/false for sucsses or failure */
int get_next_line(AsmDescriptor*);
/* frees all allocated memmory for the AsmDescriptor */
void free_asm_descriptor(AsmDescriptor*);
char* get_file_name_without_suffix(AsmDescriptor* ds);
/* check that the file name is valid */
int is_asm_file(char* file_name);

#endif
