#ifndef FISRS_SCAN_2
#define FISRS_SCAN_2

#define WORD_END_DELIMITER_COUNT 4
#define freeMem(a, b)                                                          \
  if (a != NULL)                                                               \
    free(a);                                                                   \
  if (b != NULL)                                                               \
  free(b)

#endif
