#include <stdlib.h>
#include <string.h>

#include "../headers/constants.h"
#include "../headers/line_paser.h"
#include "../headers/string_parsers.h"

/* commnet to test git */
int is_comment_line(char *line) {
  int isComment = false;
  char *trimmed_line = NULL;
  trimmed_line = trim(line);
  isComment = strlen(trimmed_line) > 0 && trimmed_line[0] == COMMENT_LINE_TOKEN;
  free(trimmed_line);
  return isComment;
}
