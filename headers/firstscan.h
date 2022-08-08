#ifndef ASEMBLER_FIRSTSCAN_H
#define ASEMBLER_FIRSTSCAN_H

#include "./DataTable.h"
#include "./LabelTable.h"
#include "./Translation.h"
#include "./asm_descriptor.h"
#include "./constants.h"
#include "./label.h"
#include "./main.h"
#include "string_parsers.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern AsmDescriptor *ds;

#define addword                                                                \
  word[j] = '\0';                                                              \
  j = 0;                                                                       \
  countWord++;                                                                 \
  checkWord(word);                                                             \
  strcpy(prevWord, word);                                                      \
  freeArr(word);
#define INVALID -1
extern AsmDescriptor *ds;

enum WORD_TYPE { ISDATA, ISOPCODE, ISREGISTR, ISNUMBER, UNKNOWN };
enum DATA_TYPE { DATAWORD, STRUCT, STRING, ENTRY, EXTERN};


void freeArr(char *);
void firstscan();
enum WORD_TYPE srchWord(char *);
int isOpcode(char *);
int isNumber(char *);
void checkData();
int isRegistr(char *);
int isData(char *);
int correctChar(char);
int isLabel(char *);
void addDataLable(char *, int);
void scanData(char *);
void addOperand(char *);
int addString(char *);
void checkLine(char *);
void checkWord(char *);
int checkNumberArr(char *);
int emptyArr(const char *arr);
int checkDestinationOperand(int opcode, int type);
void addPointOperand();
int correctLabelWord(char* word);
int checkSourceOperand(int opcode, int type);
void setDestinationOperand(char *bin, int);
void setSourceOperand(char *bin, int n);
int correctLabel(char *word);
void addOperand(char *);
int addString(char *);
void checkLine(char *);
void checkWord(char *);
int checkNumberArr(char *);
int emptyArr(const char *arr);
int checkHowOperand(const int);
void checkOpcode();
int checkTypeOperand(char *operand);
#endif
