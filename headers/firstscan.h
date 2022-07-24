#ifndef ASEMBLER_FIRSTSCAN_H
#define ASEMBLER_FIRSTSCAN_H

#include <stdio.h>
#include "string_parsers.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "./label.h"
#include "./main.h"
#include "./LabelTabel.h"
#include "./asm_descriptor.h"
#include "./DataTable.h"
#include "./Translation.h"
#include "./constants.h"
extern ds;


void freeArr(char*);
enum WORD_TYPE srchWord(const char*);
int isOpcode( char*);
int isNumber( char*);
int isRegistr( char*);
int isData( char*);
int correctChar(char);
int checkDestinationOperand(int opcode, int type);
void addPointOperand();
int checkSourceOperand(int opcode, int type);
void setDestinationOperand(char* bin, int );
void setSourceOperand(char* bin, int n);
int correctLabel(char* word);
void addOperand(char*);
int addString(const char* );
void checkLine(char*);
void checkWord( char*);
int checkNumberArr(char* );
int emptyArr(const char* arr);
int checkHowOperand(const int);
void checkOpcode(char*);
#endif


