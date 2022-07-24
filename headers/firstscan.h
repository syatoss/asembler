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
int isLabel( char*);
void addDataLable(char*, int);
void scanData(char*);
void addOperand(char*);
int addString(char* );
void checkLine(char*);
void checkWord( char*);
int checkNumberArr(char* );
int emptyArr(const char* arr);
#endif


