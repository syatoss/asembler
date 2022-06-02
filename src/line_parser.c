#include <string.h>

#include "../headers/constants.h"
#include "../headers/line_paser.h"
#include "../headers/string_parsers.h"

/* commnet to test git */
int is_comment_line(char* line) {
    char* trimmed_line;
    trimmed_line = trim(line);
    return strlen(trimmed_line) > 0 && trimmed_line[0] == COMMENT_LINE_TOKEN;
}
