#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/LabelTable.h"
#include "../headers/asm_descriptor.h"
#include "../headers/constants.h"
#include "../headers/line_paser.h"
#include "../headers/string_parsers.h"

/*
 * in the first scan all translations were made excluding not yet read lables
 * all errors were collected and prompted to the user
 * the only thing that is left is to go over each translation and
 * complete the translations for the lables that apear after the instruction
 * line the instructionTable data structure saves the names of the labes to be
 * completed the only thing nesssary is to go over the table and complete the
 * label adresses from the label table
 * and to go over the src code (assembly) and complete the .entry falgs for the
 * labels
 *
 * */

extern AsmDescriptor *ds;

int isEntry(char *word) { return strcpy(word, ENTRY_LABEL) == 0; }

int shouldSkipLine(char *line) {
  int shouldSkip = false;
  StrArr *words = get_line_words(line);
  shouldSkip = shouldSkip || is_comment_line(line);
  shouldSkip = shouldSkip || words->length < 2;
  shouldSkip = shouldSkip || !isEntry(words->strings[0]);
  free_str_arr(words);
  return shouldSkip;
}

char *getEntryLabelNameFromLine(char *line) {
  StrArr *lineWords = get_line_words(line);
  char *labelName = cp_string(lineWords->strings[1]);
  free_str_arr(lineWords);
  return labelName;
}

void setInternalLabel(char *line) {
  char *labelName;
  Label *label;
  labelName = getEntryLabelNameFromLine(line);
  label = getLabelByName(ds->lable_tb, labelName);
  label->status = INTERNAL;
}

void handleAsmLine() {
  char *line = trim(ds->line);
  if (shouldSkipLine(line))
    return;
  setInternalLabel(line);
}

void completeEntryLabelsFromSrcCode(AsmDescriptor *ds) {
  while (get_next_line(ds)) {
    handleAsmLine();
  }
}

int translationNeedsCompletion(Translation *trans) {
  int i;
  for (i = 0; i < MAX_WORDS_PER_INSTRUCTION; i++)
    if (trans->nulls[i] != NULL)
      return true;
  return false;
}

void addMissingLabelAdresses(AsmRow *row) {
  int i;
  Label *currentLabel;
  char *err;
  Translation *trans = row->translation;
  for (i = 0; i < MAX_WORDS_PER_INSTRUCTION; i++) {
    if (trans->nulls[i] == NULL)
      continue;
    currentLabel = getLabelByName(ds->lable_tb, trans->nulls[i]);
    if (!currentLabel) {
      err = cat_strings(NULL, "Error in file ", ds->file_name, " in line ",
                        row->lineNumInAsmFile, " undefined label ",
                        trans->nulls[i], NULL);
      log_error(ds->err_log, err);
      free(err);
    }
    trans->binary[i] =
        intToBinary(currentLabel->lineOfApearance +
                    ds->instructions_tb->translationCounter + START_ADDRESS);
    trans->base32[i] = binaryToBase32(trans->binary[i]);
  }
}

void completeRowTranslation(AsmRow *row) {
  if (translationNeedsCompletion(row->translation))
    addMissingLabelAdresses(row);
}

void completeInstructionTranslation(AsmDescriptor *ds) {
  forEachRow(ds->instructions_tb, completeRowTranslation);
}

void writeLabelApearanceLineToFile(Label *label, FILE *targetFile) {
  char *lineToWrite;
  int padding;
  char *base32LabelAddress;
  padding = START_ADDRESS;
  padding += label->type == DATA ? ds->instructions_tb->translationCounter : 0;
  base32LabelAddress = intToBase32(label->lineOfApearance + padding);
  lineToWrite = cat_strings(NULL, label->name, " ", base32LabelAddress, NULL);
  fputs(lineToWrite, targetFile);
  free(lineToWrite);
  free(base32LabelAddress);
}

void writeEntryFile(AsmDescriptor *ds) {
  char *entriesFileName;
  FILE *entriesFile = NULL;
  LabelNode *currentRow = NULL;
  entriesFileName = cat_strings(NULL, ds->file_name, ENTRY_FILE_SUFFIX, NULL);
  entriesFile = fopen(entriesFileName, "w");
  currentRow = ds->lable_tb->rows->head;
  while (currentRow) {
    if (currentRow->label->status == INTERNAL)
      writeLabelApearanceLineToFile(currentRow->label, entriesFile);
    currentRow = currentRow->next;
  }
  free(entriesFileName);
  fclose(entriesFile);
}

void writeExtrenalsFile(AsmDescriptor *ds) {
  char *externalsFileName;
  FILE *externalsFile = NULL;
  LabelNode *currentRow = NULL;
  externalsFileName =
      cat_strings(NULL, ds->file_name, EXTERN_FILE_SUFFIX, NULL);
  externalsFile = fopen(externalsFileName, "w");
  currentRow = ds->lable_tb->rows->head;
  while (currentRow) {
    if (currentRow->label->status == EXTERNAL)
      writeLabelApearanceLineToFile(currentRow->label, externalsFile);
    currentRow = currentRow->next;
  }
  free(externalsFileName);
  fclose(externalsFile);
}

void writeRowTranslationToFile(AsmRow *row, FILE *targetFile,
                               int startingAddress) {
  char *lineToWrite = NULL;
  char *base32Address = NULL;
  int currentMemoryWord = 0;
  for (currentMemoryWord = 0; currentMemoryWord < row->lineCount;
       currentMemoryWord++) {
    base32Address =
        intToBase32(startingAddress + row->startLine + currentMemoryWord);
    lineToWrite =
        cat_strings(NULL, base32Address, " ",
                    row->translation->base32[currentMemoryWord], NULL);
    fputs(lineToWrite, targetFile);
    free(lineToWrite);
    free(base32Address);
    lineToWrite = NULL;
    base32Address = NULL;
  }
}

void writeAsmTranslationTableToFile(AsmTranslationTable *table,
                                    FILE *targetFile, int startingAddress) {
  AsmRowNode *currentRow;
  currentRow = table->rows->head;
  while (currentRow) {
    writeRowTranslationToFile(currentRow->data, targetFile, startingAddress);
    currentRow = currentRow->next;
  }
}

void writeInstructionAndDataLengthsToFile(AsmDescriptor *ds, FILE *targetFile) {
  char *base32InstructionLenght;
  char *base32DataLenght;
  char *lineToWrite;
  base32InstructionLenght =
      intToBase32(ds->instructions_tb->translationCounter);
  base32DataLenght = intToBase32(ds->data_tb->translationCounter);
  lineToWrite =
      cat_strings(NULL, base32InstructionLenght, " ", base32DataLenght);
  fputs(lineToWrite, targetFile);
  free(base32InstructionLenght);
  free(base32DataLenght);
  free(lineToWrite);
}

void writeMachineCodeFile(AsmDescriptor *ds) {
  char *outputFileName;
  FILE *outputFile;
  int padding;
  outputFileName = cat_strings(NULL, ds->file_name, OBJECT_FILE_SUFFIX, NULL);
  outputFile = fopen(outputFileName, "w");
  padding = START_ADDRESS;
  writeInstructionAndDataLengthsToFile(ds, outputFile);
  writeAsmTranslationTableToFile(ds->instructions_tb, outputFile, padding);
  padding += ds->instructions_tb->translationCounter;
  writeAsmTranslationTableToFile(ds->data_tb, outputFile, padding);
}

void writeAsmOutput(AsmDescriptor *ds) {
  if (ds->lable_tb->hasEntryLabels)
    writeEntryFile(ds);
  if (ds->lable_tb->hasExternalLabels)
    writeExtrenalsFile(ds);
  writeMachineCodeFile(ds);
}

void secondScan() {
  completeEntryLabelsFromSrcCode(ds);
  completeInstructionTranslation(ds);
  writeAsmOutput(ds);
}
