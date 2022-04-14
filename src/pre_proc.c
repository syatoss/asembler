#include <stdio.h>
#include <stdlib.h>
#include "../headers/master_header.h"


void generate_spread_file_from(char* file_path) {
    FILE *fp;
    char* line;
    fp = fopen(file_path, "r" );
    line = get_next_line(fp);
    printf("%s", line);
    free(line);
    fclose(fp); /*just in case*/
}
