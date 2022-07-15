#ifndef ASM_DESCRIPTOR_H
#define ASM_DESCRIPTOR_H

#include <stdio.h>

#include "./DataTable.h"
#include "./LabelTable.h"
#include "./error_logger.h"
#include "./utils.h"

typedef struct {
    FILE* fp;
    char* file_name; /* line in the assembly file*/
    char* line;      /* line in the assembly file*/
    int line_num;
    ErrorLogger* err_log;
    LabelTable* lable_tb;
    AsmTranslationTable* instructions_tb;
    AsmTranslationTable* data_tb;
} AsmDescriptor;

/* creates a new struct that holds all the information needed for the assembler
 * to run */
AsmDescriptor* new_asm_descriptor(char*);
/* reads the next line from the file in the AsmDescriptor and assignes it to the
 * line field in the variable, trurn true/false rather read was sucssefull false
 * means file is done */
int get_next_line(AsmDescriptor*);
/* frees all allocated memmory for the AsmDescriptor */
void free_asm_descriptor(AsmDescriptor*);
char* get_file_name_without_suffix(AsmDescriptor* ds);
/* check that the file name is valid */
int is_asm_file(char* file_name);

#endif
