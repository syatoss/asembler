#include "../headers/lineFlags.h"
#include "../headers/constants.h"
#include <stdlib.h>

LineFlags *newLineFlags() {
  LineFlags *flags = (LineFlags *)malloc(sizeof(LineFlags));
  clearFlags(flags);
  return flags;
}

void clearFlags(LineFlags *flags) {
  flags->hasLabelDef = false;
  flags->isDataDef = false;
  flags->isStringDef = false;
  flags->isExtern = false;
  flags->isEntry = false;
  flags->is2OpreandInstruction = false;
  flags->is1OperandInstruction = false;
  flags->is0operandInstruction = false;
}

void freeLineFlags(LineFlags *flags) {
  if (flags != NULL)
    free(flags);
}
