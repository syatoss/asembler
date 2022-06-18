#include "../headers/asm_descriptor.h"

#include <stdlib.h>
#include <string.h>

#include "../headers/constants.h"
#include "../headers/string_parsers.h"

AsmDescriptor* new_asm_descriptor(char* file_path) {
    AsmDescriptor* ds;
    ds = (AsmDescriptor*)malloc(sizeof(AsmDescriptor));
    ds->fp = fopen(file_path, "r");
    if (ds->fp == NULL) return NULL;
    ds->line = (char*)malloc(sizeof(char) * STRING_BUFFER_SIZE);
    ds->file_name = (char*)malloc(STRING_BUFFER_SIZE * sizeof(char));
    if (ds->line == NULL || ds->file_name == NULL) SYS_MEM_FAIL_EXIT(1);
    strcpy(ds->file_name, file_path);
    ds->line_num = 0;
    ds->err_log = new_error_logger(stderr);
    return ds;
}

int get_next_line(AsmDescriptor* ds) {
    char* line;
    int was_successful = false;
    line = (char*)malloc(sizeof(char) * STRING_BUFFER_SIZE);
    line = fgets(line, STRING_BUFFER_SIZE, ds->fp);
    was_successful = line == NULL ? false : true;
    if (was_successful) {
        ds->line_num++;
        strcpy(ds->line, line);
        free(line);
    }
    return was_successful;
}

int is_asm_file(char* file_name) {
    char* suffix_start;
    int suffix_len;
    int file_name_len;
    file_name_len = strlen(file_name);
    suffix_len = strlen(ASM_FILE_SUFFIX);
    if (file_name_len < suffix_len) return false;
    suffix_start = file_name + (file_name_len - suffix_len - 1);
    return strcmp(suffix_start, ASM_FILE_SUFFIX) == 0;
}

char* get_file_name_without_suffix(AsmDescriptor* ds) {
    int file_name_len;
    int suffix_len;
    char* file_name_without_suffix;
    file_name_len = strlen(ds->file_name);
    suffix_len = strlen(ASM_FILE_SUFFIX);
    file_name_without_suffix = cp_string(ds->file_name);
    file_name_without_suffix[file_name_len - suffix_len] = '\0';
    return file_name_without_suffix;
}

void free_asm_descriptor(AsmDescriptor* ds) {
    if (ds == NULL) return;
    if (ds->line != NULL) free(ds->line);
    if (ds->fp != NULL) fclose(ds->fp);
    if (ds->err_log != NULL) clear_logger(ds->err_log);
    free(ds);
}
