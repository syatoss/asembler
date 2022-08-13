#include "../headers/lineFlags.h"
#include "../headers/constants.h"
#include <stdlib.h>

LineFlags *newLineFlags() {
  LineFlags *flags = (LineFlags *)malloc(sizeof(LineFlags));
  clearFlags(flags);
  return flags;
}

void clearFlags(LineFlags *flags) { flags->hasLabelDef = false; }

void freeLineFlags(LineFlags *flags) {
  if (flags != NULL)
    free(flags);
}
