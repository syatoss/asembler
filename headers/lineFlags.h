#ifndef LINE_FLAGS_H
#define LINE_FLAGS_H

typedef struct {
  int hasLabelDef;
  int isDataDef;
  int isStringDef;
  int isExtern;
  int isEntry;
  int is2OpreandInstruction;
  int is1OperandInstruction;
  int is0operandInstruction;
} LineFlags;

LineFlags *newLineFlags();
void clearFlags(LineFlags *flags);
void freeLineFlags(LineFlags *flags);

#endif
