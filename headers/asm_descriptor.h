#include <stdio.h>
#include "./utils.h"
#include "./error_logger.h"

typedef struct {
    FILE* fp;
    char* file_name;
    char* line;
    int line_num;
    error_logger* err_log;
} asm_descriptor;



asm_descriptor* new_asm_descriptor(char*);
int get_next_line(asm_descriptor*);
void free_asm_descriptor(asm_descriptor*);

