#ifndef OPERAND_LBA_H
#define OPERAND_LBA_H

enum LBA { IMEDIATE_ACCESS, DIRECT_ACCESS, DATA_ACCESS, REGISTER_ACCESS };

typedef struct {
  int imediteAccess;  /* 0 */
  int directAccess;   /* 1 */
  int dataAccess;     /* 2 */
  int registerAccess; /* 3 */
} OperandLBA;         /* logical block addressing */

typedef struct {
  OperandLBA *firstOperand;
  OperandLBA *secondOperand;
} InstuctionAllowedOpreandLBA;

void clearOperandLBA(OperandLBA *lba);
OperandLBA *newOperandLBA();
void freeOperandLBA(OperandLBA *lba);
void setOperandLBA(OperandLBA *operandLBA, ...);
InstuctionAllowedOpreandLBA *newInstructionAllowedOperandLBA();
InstuctionAllowedOpreandLBA *getAllowedLBAForInstruction(char *instructionName);
void freeInstuctionAllowedOpreandLBA(InstuctionAllowedOpreandLBA *allowedLBA);

#endif
