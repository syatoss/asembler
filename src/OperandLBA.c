#include "../headers/OperandLBA.h"
#include "../headers/constants.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void clearOperandLBA(OperandLBA *lba) {
  lba->imediteAccess = false;  /* 0 */
  lba->directAccess = false;   /* 1 */
  lba->dataAccess = false;     /* 2 */
  lba->registerAccess = false; /* 3 */
}

OperandLBA *newOperandLBA() {
  OperandLBA *lba = (OperandLBA *)malloc(sizeof(OperandLBA));
  clearOperandLBA(lba);
  return lba;
}

void setOperandLBA(OperandLBA *operandLBA, ...) {
  va_list nextArg;
  int lba = 0;
  clearOperandLBA(operandLBA);
  va_start(nextArg, operandLBA);
  while ((lba = va_arg(nextArg, int)) != INVALID) {
    if (lba == IMEDIATE_ACCESS)
      operandLBA->imediteAccess = true;
    if (lba == DATA_ACCESS)
      operandLBA->dataAccess = true;
    if (lba == DIRECT_ACCESS)
      operandLBA->directAccess = true;
    if (lba == REGISTER_ACCESS)
      operandLBA->registerAccess = true;
  }
  va_end(nextArg);
}

void freeOperandLBA(OperandLBA *lba) {
  if (lba != NULL) {
    free(lba);
  }
}

InstuctionAllowedOpreandLBA *newInstructionAllowedOperandLBA() {
  InstuctionAllowedOpreandLBA *allowedLBA =
      (InstuctionAllowedOpreandLBA *)malloc(
          sizeof(InstuctionAllowedOpreandLBA));
  allowedLBA->firstOperand = NULL;
  allowedLBA->secondOperand = NULL;
  return allowedLBA;
}

void freeInstuctionAllowedOpreandLBA(InstuctionAllowedOpreandLBA *allowedLBA) {
  if (allowedLBA == NULL)
    return;
  freeOperandLBA(allowedLBA->firstOperand);
  freeOperandLBA(allowedLBA->secondOperand);
  free(allowedLBA);
}

InstuctionAllowedOpreandLBA *
getAllowedLBAForInstruction(char *instructionName) {
  InstuctionAllowedOpreandLBA *instructionLBA =
      newInstructionAllowedOperandLBA();
  if (strcmp(instructionName, "mov") == 0) {
    instructionLBA->firstOperand = newOperandLBA();
    instructionLBA->secondOperand = newOperandLBA();
    setOperandLBA(instructionLBA->firstOperand, 0, 1, 2, 3, -1);
    setOperandLBA(instructionLBA->secondOperand, 1, 2, 3, -1);
  }
  if (strcmp(instructionName, "cmp") == 0) {
    instructionLBA->firstOperand = newOperandLBA();
    instructionLBA->secondOperand = newOperandLBA();
    setOperandLBA(instructionLBA->firstOperand, 0, 1, 2, 3, -1);
    setOperandLBA(instructionLBA->secondOperand, 0, 1, 2, 3, -1);
  }
  if (strcmp(instructionName, "add") == 0) {
    instructionLBA->firstOperand = newOperandLBA();
    instructionLBA->secondOperand = newOperandLBA();
    setOperandLBA(instructionLBA->firstOperand, 0, 1, 2, 3, -1);
    setOperandLBA(instructionLBA->secondOperand, 1, 2, 3, -1);
  }
  if (strcmp(instructionName, "sub") == 0) {
    instructionLBA->firstOperand = newOperandLBA();
    instructionLBA->secondOperand = newOperandLBA();
    setOperandLBA(instructionLBA->firstOperand, 0, 1, 2, 3, -1);
    setOperandLBA(instructionLBA->secondOperand, 1, 2, 3, -1);
  }
  if (strcmp(instructionName, "not") == 0) {
    instructionLBA->secondOperand = newOperandLBA();
    setOperandLBA(instructionLBA->secondOperand, 1, 2, 3, -1);
  }
  if (strcmp(instructionName, "clr") == 0) {
    instructionLBA->secondOperand = newOperandLBA();
    setOperandLBA(instructionLBA->secondOperand, 1, 2, 3, -1);
  }
  if (strcmp(instructionName, "lea") == 0) {
    instructionLBA->firstOperand = newOperandLBA();
    instructionLBA->secondOperand = newOperandLBA();
    setOperandLBA(instructionLBA->firstOperand, 1, 2, -1);
    setOperandLBA(instructionLBA->secondOperand, 1, 2, 3, -1);
  }
  if (strcmp(instructionName, "inc") == 0) {
    instructionLBA->secondOperand = newOperandLBA();
    setOperandLBA(instructionLBA->secondOperand, 1, 2, 3, -1);
  }
  if (strcmp(instructionName, "dec") == 0) {
    instructionLBA->secondOperand = newOperandLBA();
    setOperandLBA(instructionLBA->secondOperand, 1, 2, 3, -1);
  }
  if (strcmp(instructionName, "jmp") == 0) {
    instructionLBA->secondOperand = newOperandLBA();
    setOperandLBA(instructionLBA->secondOperand, 1, 2, 3, -1);
  }
  if (strcmp(instructionName, "bne") == 0) {
    instructionLBA->secondOperand = newOperandLBA();
    setOperandLBA(instructionLBA->secondOperand, 1, 2, 3, -1);
  }
  if (strcmp(instructionName, "get") == 0) {
    instructionLBA->secondOperand = newOperandLBA();
    setOperandLBA(instructionLBA->secondOperand, 1, 2, 3, -1);
  }
  if (strcmp(instructionName, "prn") == 0) {
    instructionLBA->secondOperand = newOperandLBA();
    setOperandLBA(instructionLBA->secondOperand, 0, 1, 2, 3, -1);
  }
  if (strcmp(instructionName, "jsr") == 0) {
    instructionLBA->secondOperand = newOperandLBA();
    setOperandLBA(instructionLBA->secondOperand, 1, 2, 3, -1);
  }
  if (strcmp(instructionName, "rts") == 0) {
    /* no operands allowed */
  }
  if (strcmp(instructionName, "hlt") == 0) {
    /* no operands allowed */
  }
  return instructionLBA;
}
