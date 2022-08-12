#ifndef FISRS_SCAN_2
#define FISRS_SCAN_2

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

void firstScan(AsmDescriptor *ds);
char *getNextWordWithWordEndDelimiter(char *line, int *lastReadCharIndex);
int isLabelDef(char *word);

#endif
