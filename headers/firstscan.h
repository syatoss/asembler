#ifndef ASEMBLER_FIRSTSCAN_H
#define ASEMBLER_FIRSTSCAN_H

#include <stdio.h>
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
char* srchWord(char*);
int isOpcode(char*);
int isNumber(char*);
int isRegistr(char*);
int isData(char*);
int isLabel(char*);
void addDataLable(char* , int);
void checkLine(char*);
void checkWord(char*);
int checkNumberArr(char* );
#endif //ASEMBLER_FIRSTSCAN_H


