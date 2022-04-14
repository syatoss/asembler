#include <stdio.h>
#include <stdlib.h>
#include "../headers/master_header.h"
#include "../headers/constants.h"

char* get_next_line(FILE* fp) {
    char* line;
    int total_alloc = STRING_BUFFER_SIZE;
    int eol_offset = 0;
    line = malloc(STRING_BUFFER_SIZE * sizeof(char));
    fgets(line, STRING_BUFFER_SIZE, fp);
    if(line_break_made(line)) {
        eol_offset = end_line_on_break(line);
    }
    return line;
}

int line_break_made(char* line) {
    int i = 0;
    while(line[i] != '\0') {
        if(line[i] == '\n') return true;
        i++;
    }
    return false;
}

int end_line_on_break(char* line) {
    int i = 0;
    int j = 0;
    while(line && line[i] != '\n') i++;
    line[i] = '\0';
    j = i;
    i++;
    while(line[i] != '\0') i++;
    return i-j;

}
