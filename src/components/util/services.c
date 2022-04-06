#include <stdio.h>
#include <stdlib.h>
#include "./../../headers/master_header.h"
#include "../../constants.h"

void read_next_line(FILE* fp, char* line) {
    free(line);
    line = malloc(STRING_BUFFER_SIZE * sizeof(char));
    fgets(line, STRING_BUFFER_SIZE, fp);
}
