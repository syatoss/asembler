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

/* allocates new memmory for OperandLBA assinges default values and returns the
 * pointer */
OperandLBA *newOperandLBA();
/* sets default values to lba */
void clearOperandLBA(OperandLBA *lba);
/* frees the pointer and all assosiated allocated memory */
void freeOperandLBA(OperandLBA *lba);
/* sets the operandLBA values to the provided values, recieves -1 as last
 * parameter */
void setOperandLBA(OperandLBA *operandLBA, ...);
/* allocates new memmory for InstuctionAllowedOpreandLBA  assinges default
 * values and returns the pointer */
InstuctionAllowedOpreandLBA *newInstructionAllowedOperandLBA();
/* returns the allowed LBA for the provided instruction */
InstuctionAllowedOpreandLBA *getAllowedLBAForInstruction(char *instructionName);
/* frees the pointer and all assosiated allocated memory */
void freeInstuctionAllowedOpreandLBA(InstuctionAllowedOpreandLBA *allowedLBA);

#endif
