#include <stdlib.h>
#include <string.h>
#include "../headers/asm_descriptor.h"
#include "../headers/constants.h"

AsmDescriptor* new_asm_descriptor(char* file_path) {
    AsmDescriptor* ds;
    ds  = (AsmDescriptor*)malloc(sizeof(AsmDescriptor));
    ds->fp = fopen(file_path, "r" );
    if(ds->fp == NULL) return NULL;
    ds->line = (char*)malloc(sizeof(char) * STRING_BUFFER_SIZE);
    ds->file_name = (char*)malloc(STRING_BUFFER_SIZE * sizeof(char));
    if(ds->line == NULL || ds->file_name == NULL) SYS_MEM_FAIL_EXIT(1);
    strcpy(ds->file_name, file_path);
    ds->line_num = 0;
    ds->err_log = new_error_logger(stderr);
    return ds;
}

int get_next_line(AsmDescriptor* ds) {
    int was_successful = false;
    ds->line = fgets(ds->line, STRING_BUFFER_SIZE, ds->fp);
    was_successful = ds->line == NULL? false : true;
    if(was_successful) ds->line_num++;
    return was_successful;
}

void free_asm_descriptor(AsmDescriptor* ds) {
    if(ds == NULL) return;
    if(ds->line != NULL) free(ds->line);
    if(ds->fp != NULL) fclose(ds->fp);
    if(ds->err_log != NULL) clear_logger(ds->err_log);
    free(ds);
}
