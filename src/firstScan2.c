#include "../headers/firstScan2.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/asm_descriptor.h"
#include "../headers/constants.h"
#include "../headers/error_logger.h"
#include "../headers/line_paser.h"
#include "../headers/string_parsers.h"

const char WORD_END_DELIMITERS[WORD_END_DELIMITER_COUNT] = {'\n', '\0', ' ',
                                                            '\t'};
const char *OPERATORS[NUM_OF_OPERATORS] = {
    "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
    "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};
const char *REGISTERS[NUM_OF_REGISTERS] = {"r0", "r1", "r2", "r3",
                                           "r4", "r5", "r6", "r7"};
const char *DATA_TYPES[NUM_OF_DATA_TYPES] = {
    ".data",
    ".struct",
    ".string",
};
const char *LABEL_STATUSES[NUM_OF_LABEL_STATUSES] = {EXTERN_LABEL, ENTRY_LABEL};

extern AsmDescriptor *ds;

int isWordEndDelimiter(char c) {
  int i;
  for (i = 0; i < WORD_END_DELIMITER_COUNT; i++) {
    if (c == WORD_END_DELIMITERS[i])
      return true;
  }
  return false;
}

int isInstructionName(char *word) {
  int i;
  int isOperator = false;
  word = trim(word);
  for (i = 0; i < NUM_OF_OPERATORS; i++) {
    isOperator = isOperator || (strcmp(word, OPERATORS[i]) == 0);
  }
  free(word);
  return isOperator;
}

int isDataDef(char *word) {
  int isData = false;
  int i;
  word = trim(word);
  for (i = 0; i < NUM_OF_DATA_TYPES; i++) {
    isData = isData || (strcmp(word, DATA_TYPES[i]) == 0);
  }
  free(word);
  return isData;
}

char *getNextWordWithWordEndDelimiter(char *line, int *lastReadCharIndex) {
  char *word;
  int startIndex;
  while (is_white_space(line[*lastReadCharIndex]))
    (*lastReadCharIndex)++;
  startIndex = *lastReadCharIndex;
  while (!isWordEndDelimiter(line[(*lastReadCharIndex)++]))
    ;
  word = substring(line, startIndex, *lastReadCharIndex - startIndex);
  return word;
}

int isEntryExtern(char *word) {
  int isEntOrExt = false;
  int i;
  word = trim(word);
  for (i = 0; i < NUM_OF_LABEL_STATUSES; i++) {
    isEntOrExt = isEntOrExt || (strcmp(word, LABEL_STATUSES[i]) == 0);
  }
  free(word);
  return isEntOrExt;
}

int isRegister(char *word) {
  int isRegister = false;
  int i;
  word = trim(word);
  for (i = 0; i < NUM_OF_REGISTERS; i++) {
    isRegister = isRegister || (strcmp(word, REGISTERS[i]) == 0);
  }
  free(word);
  return isRegister;
}

int isLanguageReservedWord(char *word) {
  return isRegister(word) || isInstructionName(word) || isEntryExtern(word) ||
         isDataDef(word);
}

int isLabelDef(char *word) {
  if (word[strlen(word) - 1] != ' ')
    return false;
  word = trim(word);
  if (word[strlen(word) - 1] != ':')
    return false;
  if (isLanguageReservedWord(word))
    return false;
  free(word);
  return true;
}

enum DATA_TYPES getDataType(char *dataType) {
  if (strcmp(dataType, ".data") == 0)
    return NUMBER;
  if (strcmp(dataType, ".string") == 0)
    return STRING;
  return STRUCT;
}

void readEntryExtern(char *line, Label *currentLabel) {}

void setLabelDefFlag(AsmDescriptor *ds, int isLabelDef) {}

int getLabelDefFlag() { return true; }

void pushInt(int **intArrPtr, int num, int arrSize) {
  int *new = (int *)realloc(*intArrPtr, arrSize + 1);
  *intArrPtr = new;
}

int isInt(char *word) {
  int i = 0;
  int len;
  len = strlen(word);
  if (len < 1)
    return false;
  if (word[i] == '+' || word[i] == '-')
    i++;
  for (; i < len; i++) {
    if (!isdigit(word[i]))
      return false;
  }
  return true;
}

int getLineValidityForDataDef(char *line) {
  int delimiterExpected = false;
  int currentCharIndex = 0;
  int numberExpected = true;
  int currentWordCharIndex = 0;
  char delimiter = ',';
  char currentWord[MAX_LABEL_LENGHT] = {0};
  while (is_white_space(line[currentCharIndex]))
    currentCharIndex++;
  for (; line[currentCharIndex] != '\n' && line[currentCharIndex] != '\0';
       currentCharIndex++) {
    if (numberExpected) {
      /* if space is read its assumed that the next word is read */
      if (line[currentCharIndex] == ' ') {
        delimiterExpected = true;
        numberExpected = false;
        currentWord[currentWordCharIndex] = '\0';
        currentWordCharIndex = 0;
        if (!isInt(currentWord))
          return false;
        continue;
      }
      /* if unexpected delimiter is encountered than the numver def is invalid*/
      if (line[currentCharIndex] == delimiter && !delimiterExpected)
        return false;
      /* if an expected delimiter is encountered the current word is regarded as
       * copletely read */
      else if (line[currentCharIndex] == delimiter && delimiterExpected) {
        currentWord[currentWordCharIndex++] = '\0';
        currentWordCharIndex = 0;
        if (!isInt(currentWord))
          return false;
        delimiterExpected = false;
        numberExpected = true;
        continue;
      }
      currentWord[currentWordCharIndex++] = line[currentCharIndex];
      /*epxect delimiter if a whole nuber could have been read*/
      delimiterExpected = ((currentWord[0] != '+' && currentWord[0] != '-') ||
                           currentWordCharIndex > 1);
      continue;
    }
    if (delimiterExpected && !numberExpected) {
      if (line[currentCharIndex] == ' ') {
        continue;
      }
      if (line[currentCharIndex] != ',')
        return false;
      delimiterExpected = false;
      numberExpected = true;
    }
  }
  /* checks that the the line did not end in a delimiter */
  return delimiterExpected;
}

StrArr *getNumbersFromLine(char *line, int *lastReadCharIndex) {
  int validDataDef = false;
  validDataDef = getLineValidityForDataDef(line + *lastReadCharIndex);
  if (!validDataDef)
    return NULL;
  return split(line, ",");
  /* int expectSeperator = false; */
  /* char currntNumberString[MAX_LABEL_LENGHT]; */
  /* int currentNumberStringIndex = 0; */
  /* int singRead = false; */
  /* int numCount = 0; */
  /* int currntNumberInt; */
  /* int *numbers = NULL; */
  /* while (is_white_space(line[*lastReadCharIndex])) */
  /*   (*lastReadCharIndex)++; */
  /* while (line[*lastReadCharIndex] != '\0' || line[*lastReadCharIndex] !=
   * '\n') { */
  /*   while (is_white_space(line[*lastReadCharIndex])) */
  /*     (*lastReadCharIndex)++; */
  /*   if (expectSeperator) { */
  /*     if (line[*lastReadCharIndex] != ',') { */
  /*       printf("missing \",\" in data definition"); */
  /*     } */
  /*     (*lastReadCharIndex)++; */
  /*     expectSeperator = false; */
  /*     continue; */
  /*   } */

  /*   if ((line[*lastReadCharIndex] == '-' || line[*lastReadCharIndex] == '+')
   * && */
  /*       !singRead) { */
  /*     currntNumberString[currentNumberStringIndex] =
   * line[*lastReadCharIndex]; */
  /*     currentNumberStringIndex++; */
  /*     (*lastReadCharIndex)++; */
  /*     singRead = true; */
  /*   } */
  /*   if ((line[*lastReadCharIndex] == '-' || line[*lastReadCharIndex] == '+')
   * && */
  /*       singRead) { */
  /*     printf("duplicate use of sing"); */
  /*     continue; */
  /*   } */
  /*   while (isdigit(line[*lastReadCharIndex])) { */
  /*     currntNumberString[currentNumberStringIndex] =
   * line[*lastReadCharIndex]; */
  /*     currentNumberStringIndex++; */
  /*     (*lastReadCharIndex)++; */
  /*   } */
  /*   if (line[*lastReadCharIndex] == ' ') { */
  /*     expectSeperator = true; */
  /*   } */
  /*   if (line[*lastReadCharIndex] == ',') { */
  /*     expectSeperator = false; */
  /*   } */
  /*   currntNumberString[currentNumberStringIndex] = '\0'; */
  /*   currntNumberInt = atoi(currntNumberString); */
  /*   pushInt(&numbers, currntNumberInt, numCount++); */
  /*   currentNumberStringIndex = 0; */
}

/* pushInt(&numbers, INT_ARR_END_DELIMITER, numCount); */
/* return numbers; */
/* } */

void addNumbersToTranslation(StrArr *numbers, Translation *trans) {
  int i;
  int num = 0;
  for (i = 0; i < numbers->length; i++) {
    num = atoi(numbers->strings[i]);
    addTranslation(intToBinary(num), NULL, trans);
  }
}

void readRestOfNumberLine(char *line, int *lastReadCharIndex,
                          Label *currentLabel) {
  StrArr *numbers = NULL;
  char *err = NULL;
  Translation *trans;
  char *labelName = NULL;
  numbers = getNumbersFromLine(line, lastReadCharIndex);
  if (numbers == NULL) {
    err = cat_strings("Error in file ", ds->file_name, " in line ",
                      ds->line_num_string, " invalid .data definition", NULL);
    log_error(ds->err_log, err);
    freeMem(err, err);
    return;
  }
  trans = newTranslation();
  labelName = currentLabel == NULL ? NULL : currentLabel->name;
  addNumbersToTranslation(numbers, trans);
  addAsmRowToTable(newAsmRow(trans->lineCount, ds->line_num,
                             ds->data_tb->translationCounter, trans,
                             labelName != NULL, labelName),
                   ds->data_tb);
}

void readRestOfStringLine(char *line, int *lastReadCharIndex) {}
void readRestOfStructLine(char *line, int *lastReadCharIndex) {}

void readRestOfDataLine(char *line, char *dataType, int *lastReadCharIndex,
                        Label *currentLabel) {
  enum DATA_TYPES type = getDataType(dataType);
  switch (type) {
  case NUMBER:
    readRestOfNumberLine(line, lastReadCharIndex, currentLabel);
    break;
  case STRING:
    readRestOfStringLine(line, lastReadCharIndex);
    break;
  case STRUCT:
    readRestOfStructLine(line, lastReadCharIndex);
    break;
  default:
    break;
  }
}

void readRestOfInstruction(char *line, int *lastReadCharIndex,
                           Label *currentLabel) {}

void firstScan(AsmDescriptor *ds) {
  char *word = NULL;
  char *labelName = NULL;
  Label *currentLabel = NULL;
  int lastReadCharIndex = 0;
  char *err = NULL;
  while (get_next_line(ds)) {
    if (is_comment_line(ds->line))
      continue;

    word = getNextWordWithWordEndDelimiter(ds->line, &lastReadCharIndex);
    currentLabel = NULL;

    if (isLabelDef(word)) {
      setLabelDefFlag(ds, true);
      labelName = getLabelNameFromDefinition(word);
      if (getLabelByName(ds->lable_tb, labelName)) {
        err = cat_strings("Error in file ", ds->file_name, " in line ",
                          ds->line_num_string, " duplicate definition of \"",
                          labelName, "\"", NULL);
        log_error(ds->err_log, err);
        freeMem(err, labelName);
        continue;
      }
      word = getNextWordWithWordEndDelimiter(ds->line, &lastReadCharIndex);
      currentLabel = newLabel(labelName, ds->line_num, NONE, DATA);
    }

    if (isEntryExtern(word)) {
      readEntryExtern(ds->line, currentLabel);
      freeMem(word, labelName);
      continue;
    }

    if (isDataDef(word)) {
      if (getLabelDefFlag() == true)
        currentLabel->type = DATA;
      readRestOfDataLine(ds->line, word, &lastReadCharIndex, currentLabel);
      addLabelToTable(currentLabel, ds->lable_tb);
      freeMem(word, labelName);
      continue;
    }

    if (isInstructionName(word)) {
      if (getLabelDefFlag() == true)
        currentLabel->type = INSTRUCTION;
      readRestOfInstruction(ds->line, &lastReadCharIndex, currentLabel);
      freeMem(word, labelName);
      continue;
    } else {
      err = cat_strings("Error in file ", ds->file_name, " in line ",
                        ds->line_num_string, " invalid label definition", NULL);

      log_error(ds->err_log, err);
      freeMem(err, err);
    }
    freeMem(word, labelName);
  }
}
