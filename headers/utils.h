#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#define SYS_MEM_FAIL_EXIT(exit_status)             \
    {                                              \
        (printf("Memmory allocation failure \n")); \
        exit(exit_status);                         \
    }

#endif
