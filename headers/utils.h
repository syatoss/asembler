#include <stdio.h>
#define SYS_MEM_FAIL_EXIT(exit_status) printf("Memmory allocation failure"); \
    exit(exit_status);

typedef struct {
    FILE* fp;
    char* file_name;
    char* line;
    int line_num;
} asm_descriptor;



