#include <stdlib.h>
#include "../headers/asm_descriptor.h"
#include "../headers/constants.h"

asm_descriptor* new_asm_descriptor(char* file_path) {
    asm_descriptor* ds;
    ds  = malloc(sizeof(asm_descriptor));
    ds->fp = fopen(file_path, "r" );
    if(!ds->fp) return NULL;
    ds->line = malloc(sizeof(char)* STRING_BUFFER_SIZE);
    ds->file_name = file_path;
    ds->line_num = 0;
    return ds;
}

int get_next_line(asm_descriptor* ds) {
    int was_successful = false;
    ds->line = fgets(ds->line, STRING_BUFFER_SIZE, ds->fp);
    was_successful = ds->line == NULL? false : true;
    if(was_successful) ds->line_num++;
    return was_successful;
}

void free_asm_descriptor(asm_descriptor* ds) {
    if(ds == NULL) return;
    if(ds->line != NULL) free(ds->line);
    if(ds->fp != NULL) fclose(ds->fp);
    free(ds);
}
