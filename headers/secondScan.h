#ifndef SECOND_SCAN_H
#define SECOND_SCAN_H

#include "./asm_descriptor.h"

void handleAsmLine();
void secondScan();
int isEntry(char* word);
int shouldSkipLine(char* line);
char* getEntryLabelNameFromLine(char* line);
void setInternalLabel(char* line);
void handleAsmLine();
void completeInstructionTranslation(AsmDescriptor* ds);

#endif

