#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/LabelTable.h"
#include "../headers/asm_descriptor.h"
#include "../headers/constants.h"
#include "../headers/firstScan2.h"
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

int isEntry(char *word) { return strcmp(word, ENTRY_LABEL) == 0; }

int shouldSkipLine(char *line) {
  int shouldSkip = false;
  int i = 0;
  int hasEntryDef = false;
  StrArr *words = get_line_words(line);
  shouldSkip = shouldSkip || is_comment_line(line);
  shouldSkip = shouldSkip || words->length < 2;
  /* shouldSkip = shouldSkip || !isEntry(words->strings[0]); */
  for (i = 0; i < words->length; i++)
    hasEntryDef = hasEntryDef || isEntry(words->strings[i]);
  shouldSkip = shouldSkip || !hasEntryDef;
  free_str_arr(words);
  return shouldSkip;
}

char *getEntryLabelNameFromLine(char *line) {
  char *wordInLine = NULL;
  int lastReadIndex = 0;
  StrArr *lineWords;
  char *labelName = NULL;
  wordInLine = getNextWordWithWordEndDelimiter(line, &lastReadIndex);
  if (isLabelDef(wordInLine)) {
    freeMem(wordInLine, wordInLine);
    /* should read .entry */
    wordInLine = getNextWordWithWordEndDelimiter(line, &lastReadIndex);
  }

  lineWords = get_line_words(line + lastReadIndex);
  labelName = cp_string(lineWords->strings[0]);
  free_str_arr(lineWords);
  return labelName;
}

void setInternalLabel(char *line) {
  char *labelName = NULL;
  Label *label = NULL;
  char *err = NULL;
  labelName = getEntryLabelNameFromLine(line);
  label = getLabelByName(ds->lable_tb, labelName);
  if (!label) {
    err = cat_strings("Error in file ", ds->file_name, " in line ",
                      ds->line_num_string, " label \"", labelName,
                      "\" marked as entry is not defined in file", NULL);
    log_error(ds->err_log, err);
    free(err);
    return;
  }
  label->status = INTERNAL;
  ds->lable_tb->hasEntryLabels = true;
  free(labelName);
}

void handleAsmLine() {
  char *line = trim(ds->line);
  if (shouldSkipLine(line)) {
    free(line);
    return;
  }
  setInternalLabel(line);
  free(line);
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
  char *err = NULL;
  char *rowInAsmFile = NULL;
  Translation *trans = row->translation;
  for (i = 0; i < MAX_WORDS_PER_INSTRUCTION; i++) {
    if (trans->nulls[i] == NULL)
      continue;
    currentLabel = getLabelByName(ds->lable_tb, trans->nulls[i]);
    if (!currentLabel) {
      rowInAsmFile = itoa(row->lineNumInAsmFile, 10);
      err =
          cat_strings(NULL, "Error in file ", ds->file_name, " in line ",
                      rowInAsmFile, " undefined label ", trans->nulls[i], NULL);
      log_error(ds->err_log, err);
      free(err);
      free(rowInAsmFile);
      err = NULL;
      rowInAsmFile = NULL;
      continue;
    }
    trans->binary[i] =
        intToBinary(currentLabel->lineOfApearance +
                    ds->instructions_tb->translationCounter + START_ADDRESS);
    shiftLeft(trans->binary[i], 2);
    if (currentLabel->status == EXTERNAL) {
      setARE(trans->binary[i], E);
    } else {
      setARE(trans->binary[i], R);
    }
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
  lineToWrite =
      cat_strings(NULL, label->name, " ", base32LabelAddress, "\n", NULL);
  fputs(lineToWrite, targetFile);
  free(lineToWrite);
  free(base32LabelAddress);
}

void writeEntryFile(AsmDescriptor *ds) {
  char *entriesFileName;
  FILE *entriesFile = NULL;
  LabelNode *currentRow = NULL;
  entriesFileName =
      cat_strings(NULL, ds->raw_file_name, ENTRY_FILE_SUFFIX, NULL);
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
      cat_strings(NULL, ds->raw_file_name, EXTERN_FILE_SUFFIX, NULL);
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
                    row->translation->binary[currentMemoryWord], "\n", NULL);
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
  while (currentRow != NULL) {
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
  lineToWrite = cat_strings(NULL, base32InstructionLenght, " ",
                            base32DataLenght, "\n", NULL);
  fputs(lineToWrite, targetFile);
  free(base32InstructionLenght);
  free(base32DataLenght);
  free(lineToWrite);
}

void writeMachineCodeFile(AsmDescriptor *ds) {
  char *outputFileName;
  FILE *outputFile;
  int padding;
  outputFileName =
      cat_strings(NULL, ds->raw_file_name, OBJECT_FILE_SUFFIX, NULL);
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

void testPrint() {
  printf("Instruction table: ");
  printAsmTranslationTable(ds->instructions_tb);
  printf("Data table: ");
  printAsmTranslationTable(ds->data_tb);
}

void actualSecondScan() {
  completeEntryLabelsFromSrcCode(ds);
  completeInstructionTranslation(ds);
  if (ds->err_log->has_errors) {
    print_all_logger_errors(ds->err_log);
    return;
  }
  writeAsmOutput(ds);
}

void secondScan() {
  /* testPrint(); */
  actualSecondScan();
  /* printLabelTable(ds->lable_tb); */
}
