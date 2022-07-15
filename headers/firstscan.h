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
int isOpcode(const char*);
int isNumber(const char*);
int isRegistr(const char*);
int isData(const char*);
int isLabel(const char*);
void addData(char*);
void scanData(char*);
void checkLine(char*);
void checkWord(const char*);
int checkNumberArr(char* );
int emptyArr(char* arr);
#endif


