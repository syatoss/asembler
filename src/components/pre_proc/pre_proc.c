#include <stdio.h>
#include <stdlib.h>
#include "./../../headers/master_header.h"


void generate_spread_file_from(char* file_path) {
    FILE *fp;
    char* line;
    fopen(file_path, "r" );
    read_next_line(fp, line);
    printf("%s", line);
    free(line);
    fclose(fp); /*just in case*/
}
