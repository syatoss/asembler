#ifndef FISRS_SCAN
#define FISRS_SCAN

#include "./asm_descriptor.h"

#define WORD_END_DELIMITER_COUNT 4
#define INT_ARR_END_DELIMITER 9999
#define freeMem(a, b)                                                          \
  if (a != NULL) {                                                             \
    free(a);                                                                   \
    a = NULL;                                                                  \
  }                                                                            \
  if (b != NULL) {                                                             \
    free(b);                                                                   \
    b = NULL;                                                                  \
  }

enum DATA_TYPES { NUMBER, STRING, STRUCT };

/* all first scan logic */
void firstScan(AsmDescriptor *ds);
/* gets the next word in a line, used by second scan as well */
char *getNextWordWithWordEndDelimiter(char *line, int *lastReadCharIndex);
/* checks rather a certain word is lable definition - used by second scan as
 * well */
int isLabelDef(char *word);

#endif
