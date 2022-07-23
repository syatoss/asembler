#ifndef SECOND_SCAN_H
#define SECOND_SCAN_H

#include "./asm_descriptor.h"

int isEntry(char *word);
int shouldSkipLine(char *line);
char *getEntryLabelNameFromLine(char *line);
void setInternalLabel(char *line);
void handleAsmLine();
void completeEntryLabelsFromSrcCode(AsmDescriptor *ds);
int translationNeedsCompletion(Translation *trans);
void addMissingLabelAdresses(Translation *trans);
void completeRowTranslation(AsmRow *row);
void completeInstructionTranslation(AsmDescriptor *ds);
void writeLabelApearanceLineToFile(Label *label, FILE *targetFile);
void writeEntryFile(AsmDescriptor *ds);
void writeExtrenalsFile(AsmDescriptor *ds);
void writeRowTranslationToFile(AsmRow *row, FILE *targetFile,
                               int startingAddress);
void writeAsmTranslationTableToFile(AsmTranslationTable *table,
                                    FILE *targetFile, int startingAddress);
void writeInstructionAndDataLengthsToFile(AsmDescriptor *ds, FILE *targetFile);
void writeMachineCodeFile(AsmDescriptor *ds);
void writeAsmOutput(AsmDescriptor *ds);
void secondScan();

#endif
