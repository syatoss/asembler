#include <stdio.h>
#define SYS_MEM_FAIL_EXIT(exit_status) {(printf("Memmory allocation failure")); \
    exit(exit_status);}
#define MACRO_START "macro"
#define MACRO_END "endmacro"

