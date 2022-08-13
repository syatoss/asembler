#include "../headers/firstScan2.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/OperandLBA.h"
#include "../headers/asm_descriptor.h"
#include "../headers/constants.h"
#include "../headers/error_logger.h"
#include "../headers/lineFlags.h"
#include "../headers/line_paser.h"
#include "../headers/string_parsers.h"

const char WORD_END_DELIMITERS[WORD_END_DELIMITER_COUNT] = {'\n', '\0', ' ',
                                                            '\t'};
/* const char *OPERATORS[NUM_OF_INSTRUCTIONS] = { */
/*     "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", */
/*     "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"}; */
const char *ZERO_OPERAND_INSTRUCTIONS[NUM_OF_ZERO_OPERAND_INSTRUCTIONS] = {
    "hlt", "rts"};
const char *ONE_OPERAND_INSTRUCTIONS[NUM_OF_ONE_OPERAND_INSTRUCTIONS] = {
    "not", "clr", "inc", "dec", "jmp", "bne", "get", "prn", "jsr"};
const char *TWO_OPERAND_INSTRUCTIONS[NUM_OF_TWO_OPERAND_INSTRUCTIONS] = {
    "mov", "cmp", "add", "sub", "lea"};
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

int getNubmberOfRequieredOperands(char *instructionName) {
  int i = 0;
  instructionName = trim(instructionName);
  for (i = 0; i < NUM_OF_ZERO_OPERAND_INSTRUCTIONS; i++) {
    if (strcmp(instructionName, ZERO_OPERAND_INSTRUCTIONS[i]) == 0)
      return 0;
  }
  for (i = 0; i < NUM_OF_ONE_OPERAND_INSTRUCTIONS; i++) {
    if (strcmp(instructionName, ONE_OPERAND_INSTRUCTIONS[i]) == 0)
      return 1;
  }
  for (i = 0; i < NUM_OF_TWO_OPERAND_INSTRUCTIONS; i++) {
    if (strcmp(instructionName, TWO_OPERAND_INSTRUCTIONS[i]) == 0)
      return 2;
  }
  free(instructionName);
  return INVALID;
}

int isInstructionName(char *word) {
  return getNubmberOfRequieredOperands(word) != INVALID;
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

int isValidLabelName(char *lableName) {
  int i = 0;
  int len = 0;
  int upperCase = false;
  int lowerCase = false;
  int number = false;
  if (isLanguageReservedWord(lableName))
    return false;
  len = strlen(lableName);
  if (len < 1)
    return false;
  for (i = 0; i < len; i++) {
    lowerCase = (lableName[i] >= 'a' && lableName[i] <= 'z');
    upperCase = (lableName[i] >= 'A' && lableName[i] <= 'Z');
    number = (lableName[i] >= '0' && lableName[i] <= '9');
    if (i == 0 && !lowerCase && !upperCase)
      return false;
    if (!lowerCase && !upperCase && !number)
      return false;
  }
  return true;
}

int isLabelDef(char *word) {
  if (word[strlen(word) - 1] != ' ')
    return false;
  word = trim(word);
  if (word[strlen(word) - 1] != ':')
    return false;
  word[strlen(word) - 1] = '\0'; /* removes ':' */
  if (!(strlen(word) >= 1))
    return false;
  if (!isValidLabelName(word))
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

enum LABEL_STATUS getLabelStatus(char *status) {
  if (strcmp(status, ".entry") == 0)
    return INTERNAL;
  if (strcmp(status, ".extern") == 0)
    return EXTERNAL;
  return NONE;
}

int getLineValidityForEntryExternDef(char *line) {
  StrArr *lineWords = NULL;
  int isValid = true;
  lineWords = split(line, " ");
  if (lineWords->length != 1)
    isValid = false;
  isValid = isValid && isValidLabelName(lineWords->strings[0]);
  free_str_arr(lineWords);
  return isValid;
}

void readRestOfEntryLine(char *line, int *lastReadCharIndex,
                         Label *currentLabel) {
  char *err = NULL;
  if (getLineValidityForEntryExternDef(line + *lastReadCharIndex))
    return;
  err = cat_strings("Error in file ", ds->file_name, " in line ",
                    ds->line_num_string, " invalid .entry definition", NULL);
  log_error(ds->err_log, err);
  freeMem(err, err);
}

void readRestOfExternLine(char *line, int *lastReadCharIndex,
                          Label *currentLabel) {
  char *err = NULL;
  StrArr *lineWords = NULL;
  if (!getLineValidityForEntryExternDef(line + *lastReadCharIndex)) {
    err = cat_strings("Error in file ", ds->file_name, " in line ",
                      ds->line_num_string, " invalid .extern definition", NULL);
    log_error(ds->err_log, err);
    freeMem(err, err);
    return;
  }
  lineWords = split(line + *lastReadCharIndex, " ");
  currentLabel = newLabel(lineWords->strings[0], ds->line_num, EXTERNAL, DATA);
  addLabelToTable(currentLabel, ds->lable_tb);
  free_str_arr(lineWords);
}

void readEntryExtern(char *line, char *statusStr, int *lastReadCharIndex,
                     Label *currentLabel) {
  char *err = NULL;
  enum LABEL_STATUS status;
  status = getLabelStatus(statusStr);
  if (currentLabel != NULL) {
    /* freeLabel(currentLabel); */
    currentLabel = NULL;
  }
  switch (status) {
  case INTERNAL:
    readRestOfEntryLine(line, lastReadCharIndex, currentLabel);
    break;
  case EXTERNAL:
    readRestOfExternLine(line, lastReadCharIndex, currentLabel);
    break;
  default:
    err = cat_strings("Error in file ", ds->file_name, " in line ",
                      ds->line_num_string, " invalid .extern/entry definition",
                      NULL);
    log_error(ds->err_log, err);
    freeMem(err, err);
    return;
  }
}

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
    if (word[i] > '9' || word[i] < '0')
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
  char currentWord[STRING_BUFFER_SIZE] = {0};
  while (is_white_space(line[currentCharIndex]))
    currentCharIndex++;
  for (; line[currentCharIndex] != '\n' && line[currentCharIndex] != '\0';
       currentCharIndex++) {
    if (numberExpected) {
      /* if space is read its assumed that the next word is read */
      if (line[currentCharIndex] == ' ') {
        numberExpected = !delimiterExpected;
        currentWord[currentWordCharIndex] = '\0';
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
        if (!isInt(currentWord))
          return false;
        continue;
      }
      if (line[currentCharIndex] != delimiter)
        return false;
      delimiterExpected = false;
      numberExpected = true;
      if (!isInt(currentWord))
        return false;
      currentWordCharIndex = 0;
    }
  }
  /* checks that the the line did not end in a delimiter */
  if (!delimiterExpected)
    return false;
  if (currentWordCharIndex != 0)
    return isInt(currentWord);
  return true;
}

StrArr *getNumbersFromLine(char *line, int *lastReadCharIndex) {
  int validDataDef = false;
  validDataDef = getLineValidityForDataDef(line + *lastReadCharIndex);
  if (!validDataDef)
    return NULL;
  return split(line + (*lastReadCharIndex), ",");
}

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
  free_str_arr(numbers);
}

int isValidAsciiChar(char c) {
  return (c <= 'z' && c >= 'a') || (c >= 'A' && c <= 'Z');
}

int getLineValidityForStringDef(char *line) {
  int i = 0;
  int len = 0;
  char quote = '\"';
  char *trimmed = NULL;
  char *pivot = NULL;
  trimmed = trim(line);
  pivot = trimmed;
  len = strlen(trimmed);
  if (trimmed[0] != quote || trimmed[len - 1] != quote) {
    free(pivot);
    return false;
  }
  trimmed[len - 1] = '\0';
  trimmed++;
  len = strlen(trimmed);
  for (i = 0; i < len; i++) {
    if (!isValidAsciiChar(trimmed[i])) {
      free(pivot);
      return false;
    }
  }
  free(pivot);
  return true;
}

char *getAsciiStringFromLine(char *line, int *lastReadCharIndex) {
  int validDataDef = false;
  char *trimmed = NULL;
  char *pivot = NULL;
  validDataDef = getLineValidityForStringDef(line + *lastReadCharIndex);
  if (!validDataDef)
    return NULL;
  trimmed = trim(line + *lastReadCharIndex);
  pivot = trimmed;
  trimmed[strlen(trimmed) - 1] = '\0';
  trimmed++;
  trimmed = cp_string(trimmed);
  free(pivot); /* points to old location of trimmed*/
  return trimmed;
}

void addAsciiToTranslation(char *ascii, Translation *trans) {
  int i = 0;
  int len = 0;
  len = strlen(ascii);
  for (i = 0; i < len; i++) {
    addTranslation(intToBinary(ascii[i]), NULL, trans);
  }
  addTranslation(intToBinary(0), NULL, trans);
}

void readRestOfStringLine(char *line, int *lastReadCharIndex,
                          Label *currentLabel) {
  char *ascii;
  char *err = NULL;
  Translation *trans;
  char *labelName = NULL;
  ascii = getAsciiStringFromLine(line, lastReadCharIndex);
  if (ascii == NULL) {
    err = cat_strings("Error in file ", ds->file_name, " in line ",
                      ds->line_num_string, " invalid .string definition", NULL);
    log_error(ds->err_log, err);
    freeMem(err, err);
    return;
  }
  trans = newTranslation();
  labelName = currentLabel == NULL ? NULL : currentLabel->name;
  addAsciiToTranslation(ascii, trans);
  addAsmRowToTable(newAsmRow(trans->lineCount, ds->line_num,
                             ds->data_tb->translationCounter, trans,
                             labelName != NULL, labelName),
                   ds->data_tb);
}

int getLineValidityForStructDef(char *line) {
  int valid = true;
  int i = 0;
  char delimiter = ',';
  char *trimmed = NULL;
  char *pivot = NULL;
  char *number = NULL;
  char *pivot2 = NULL;
  trimmed = trim(line);
  pivot = trimmed;
  while (trimmed[i] != delimiter)
    i++;
  number = substring(trimmed, 0, i);
  pivot2 = number;
  number = trim(number);
  valid = valid && isInt(number);
  valid = valid && getLineValidityForStringDef(
                       trimmed + i + 1); /* +1 to skip the trailing ',' */
  freeMem(pivot, pivot2);
  return valid;
}

StrArr *getStructValuesFromLine(char *line, int *lastReadCharIndex) {
  int validStructDef = false;
  StrArr *values = NULL;
  char *pivot = NULL;
  int i = 0;
  validStructDef = getLineValidityForStructDef(line + *lastReadCharIndex);
  if (!validStructDef)
    return NULL;
  values = split(line + *lastReadCharIndex, ",");
  for (i = 0; i < values->length; i++) {
    pivot = values->strings[i];
    values->strings[i] = trim(pivot);
    free(pivot);
  }
  return values;
}

void addStructValuesToTranslation(StrArr *values, Translation *trans) {
  char *ascii = NULL;
  int NUMBER_INDEX = 0;
  int STRING_INDEX = 1;
  /* fetching the string part without opening and closing quotes */
  ascii = substring(values->strings[STRING_INDEX], 1,
                    strlen(values->strings[STRING_INDEX]) - 2);
  addTranslation(intToBinary(atoi(values->strings[NUMBER_INDEX])), NULL, trans);
  addAsciiToTranslation(ascii, trans);
  free(ascii);
}

void readRestOfStructLine(char *line, int *lastReadCharIndex,
                          Label *currentLabel) {
  StrArr *values;
  char *err = NULL;
  Translation *trans;
  char *labelName = NULL;
  values = getStructValuesFromLine(line, lastReadCharIndex);
  if (values == NULL) {
    err = cat_strings("Error in file ", ds->file_name, " in line ",
                      ds->line_num_string, " invalid .struct definition", NULL);
    log_error(ds->err_log, err);
    freeMem(err, err);
    return;
  }
  trans = newTranslation();
  labelName = currentLabel == NULL ? NULL : currentLabel->name;
  addStructValuesToTranslation(values, trans);
  addAsmRowToTable(newAsmRow(trans->lineCount, ds->line_num,
                             ds->data_tb->translationCounter, trans,
                             labelName != NULL, labelName),
                   ds->data_tb);
  free_str_arr(values);
}

void readRestOfDataLine(char *line, char *dataType, int *lastReadCharIndex,
                        Label *currentLabel) {
  enum DATA_TYPES type = getDataType(dataType);
  switch (type) {
  case NUMBER:
    readRestOfNumberLine(line, lastReadCharIndex, currentLabel);
    break;
  case STRING:
    readRestOfStringLine(line, lastReadCharIndex, currentLabel);
    break;
  case STRUCT:
    readRestOfStructLine(line, lastReadCharIndex, currentLabel);
    break;
  default:
    break;
  }
}

int lineEmpyOrComment(char *line) {
  char *trimmed;
  int shouldSkip;
  trimmed = trim(line);
  shouldSkip = strlen(trimmed) == 0 || is_comment_line(line);
  free(trimmed);
  return shouldSkip;
}

int isZeroOperandInsturctionLineValid(char *line) {
  char *trimmed = NULL;
  int isValid = true;
  trimmed = trim(line);
  isValid = strlen(trimmed) == 0;
  free(trimmed);
  return isValid;
}

int isImediateAccess(char *operand) {
  char *operandTrimmed = NULL;
  int i = 0;
  char prefix = '#';
  int imediateAccess = true;
  operandTrimmed = trim(operand);
  if (operand[i] != prefix) {
    imediateAccess = false;
  }
  i++;
  if (strlen(operand + i) == 0)
    imediateAccess = false;
  imediateAccess = imediateAccess && isInt(operand + i);
  free(operandTrimmed);
  return imediateAccess;
}

int isOffset(char *number) {
  if (strlen(number) == 0)
    return false;
  if (number[0] == '+' || number[0] == '-')
    return false;
  return isInt(number);
}

int isDataAccess(char *operand) {
  int dataAccess = true;
  char *trimmedOperand = NULL;
  int LABEL_INDEX = 0;
  int OFFSET_INDEX = 1;
  StrArr *opreandParts = NULL;
  trimmedOperand = trim(operand);
  opreandParts = split(operand, ".");
  if (opreandParts == NULL || opreandParts->length != 2) {
    free_str_arr(opreandParts);
    free(trimmedOperand);
    return false;
  }
  dataAccess = dataAccess &&
               isValidLabelName(opreandParts->strings[LABEL_INDEX]) &&
               isOffset(opreandParts->strings[OFFSET_INDEX]);
  free_str_arr(opreandParts);
  free(trimmedOperand);
  return dataAccess;
}

int isDirectAccess(char *operand) {
  char *trimmed = NULL;
  int directAccess = true;
  trimmed = trim(operand);
  if (strlen(trimmed) == 0) {
    free(trimmed);
    return false;
  }
  directAccess = directAccess && isValidLabelName(trimmed);
  free(trimmed);
  return directAccess;
}

int operandMatchLBA(char *operand, OperandLBA *lba) {
  int match = false;
  if (lba == NULL && operand == NULL)
    return true;
  if (lba == NULL && operand != NULL)
    return false;
  if (lba != NULL && operand == NULL)
    return false;
  if (lba->registerAccess) {
    match = match || isRegister(operand);
  }
  if (lba->imediteAccess) {
    match = match || isImediateAccess(operand);
  }
  if (lba->dataAccess) {
    match = match || isDataAccess(operand);
  }
  if (lba->directAccess) {
    match = match || isDirectAccess(operand);
  }
  return match;
}

int isOneOperandInstructionLineValid(
    char *line, InstuctionAllowedOpreandLBA *instructionLBA) {
  int i = 0;
  char word[STRING_BUFFER_SIZE] = {0};
  int currentWordIndex = 0;
  int isValid = true;
  char *trimmed = NULL;
  while (line[i] != ' ' && line[i] != '\n' && line[i] != '\0') {
    word[currentWordIndex++] = line[i++];
  }
  word[currentWordIndex] = '\0';
  trimmed =
      trim(line + i); /* checking that the is nothing left to read in the line*/
  isValid = operandMatchLBA(word, instructionLBA->secondOperand) &&
            strlen(trimmed) == 0;
  free(trimmed);
  return isValid;
}

int isTwoOperandInstructionLineValid(
    char *line, InstuctionAllowedOpreandLBA *instructionLBA) {
  char delimiter = ',';
  int i = 0;
  char *restOfLine = NULL;
  int expectDelimiter = true;
  int currentWordIndex = 0;
  char operand1[STRING_BUFFER_SIZE] = {0};
  char operand2[STRING_BUFFER_SIZE] = {0};
  while (is_white_space(line[i]))
    i++;
  restOfLine = trim(line + i);
  if (strlen(restOfLine) == 0) {
    free(restOfLine);
    return false;
  }
  while (line[i] != ' ') {
    if (line[i] == delimiter) {
      expectDelimiter = false;
      operand1[currentWordIndex] = '\0';
      break;
    }
    operand1[currentWordIndex] = line[i];
    currentWordIndex++;
    i++;
  }
  i++;
  free(restOfLine);
  restOfLine = NULL;
  restOfLine = trim(line + i);
  if (strlen(restOfLine) == 0) {
    free(restOfLine);
    return false;
  }
  currentWordIndex = 0;
  if (expectDelimiter) {
    while (true) {
      if (line[i] == delimiter) {
        i++;
        break;
      }
      if (line[i] != ' ' && line[i] != '\t') {
        return false;
      }
      i++;
    }
  }
  while (is_white_space(line[i]))
    i++;
  while (!is_white_space(line[i]) && line[i] != '\0') {
    operand2[currentWordIndex] = line[i];
    i++;
    currentWordIndex++;
  }
  free(restOfLine);
  restOfLine = NULL;
  restOfLine = trim(line + i);
  if (strlen(restOfLine) != 0) {
    free(restOfLine);
    return false;
  }
  return operandMatchLBA(operand1, instructionLBA->firstOperand) &&
         operandMatchLBA(operand2, instructionLBA->secondOperand);
}

int getLineValidityForInstruction(char *line, int requieredOperandCount,
                                  InstuctionAllowedOpreandLBA *instructionLBA) {
  int i = 0;
  while (is_white_space(line[i]))
    i++;
  if (requieredOperandCount == 0) {
    return isZeroOperandInsturctionLineValid(line + i);
  }
  if (requieredOperandCount == 1) {
    return isOneOperandInstructionLineValid(line + i, instructionLBA);
  }
  if (requieredOperandCount == 2) {
    return isTwoOperandInstructionLineValid(line + i, instructionLBA);
  }
  return false;
}

int getInstructionId(char *instructionName) {
  if (strcmp(instructionName, "mov") == 0)
    return 0;
  if (strcmp(instructionName, "cmp") == 0)
    return 1;
  if (strcmp(instructionName, "add") == 0)
    return 2;
  if (strcmp(instructionName, "sub") == 0)
    return 3;
  if (strcmp(instructionName, "not") == 0)
    return 4;
  if (strcmp(instructionName, "clr") == 0)
    return 5;
  if (strcmp(instructionName, "lea") == 0)
    return 6;
  if (strcmp(instructionName, "inc") == 0)
    return 7;
  if (strcmp(instructionName, "dec") == 0)
    return 8;
  if (strcmp(instructionName, "jmp") == 0)
    return 9;
  if (strcmp(instructionName, "bne") == 0)
    return 10;
  if (strcmp(instructionName, "get") == 0)
    return 11;
  if (strcmp(instructionName, "prn") == 0)
    return 12;
  if (strcmp(instructionName, "jsr") == 0)
    return 13;
  if (strcmp(instructionName, "rts") == 0)
    return 14;
  if (strcmp(instructionName, "hlt") == 0)
    return 15;
  return -1;
}

void addTranslationForZeroOperands(char *instructionName, Label *currentLabel,
                                   Translation *trans) {
  char *bin = NULL;
  char *label = NULL;
  bin = intToBinary(getInstructionId(instructionName));
  shiftLeft(bin, 6);
  setARE(bin, A);
  if (currentLabel != NULL)
    label = cp_string(currentLabel->name);
  addTranslation(bin, NULL, trans);
  addAsmRowToTable(newAsmRow(1, ds->instructions_tb->translationCounter,
                             ds->line_num, trans, label != NULL, label),
                   ds->instructions_tb);
  freeMem(bin, label);
}

enum LBA getOperandLBAId(char *operand) {
  if (isRegister(operand))
    return REGISTER_ACCESS;
  if (isImediateAccess(operand))
    return IMEDIATE_ACCESS;
  if (isDataAccess(operand))
    return DATA_ACCESS;
  return DIRECT_ACCESS;
}

void addOperandTranslation(char *operand, enum LBA lba, Translation *trans,
                           int operandNmber) {
  char *bin = NULL;
  char *folowupBin = NULL;
  int LABEL_INDEX = 0;
  int OFFSET_INDEX = 1;
  int offsetFisrtOperand = 2;
  int offsetSecondOperand = 6;
  StrArr *operandParts = NULL;
  switch (lba) {

  case IMEDIATE_ACCESS:
    operand++;
    bin = intToBinary(atoi(operand));
    shiftLeft(bin, 2);
    setARE(bin, A);
    addTranslation(bin, NULL, trans);
    freeMem(bin, folowupBin);
    return;
  case DIRECT_ACCESS:
    addTranslation(bin, operand, trans);
    freeMem(bin, folowupBin);
    return;
  case DATA_ACCESS:
    operandParts = split(operand, ".");
    addTranslation(bin, operandParts->strings[LABEL_INDEX], trans);
    folowupBin = intToBinary(atoi(operandParts->strings[OFFSET_INDEX]));
    shiftLeft(folowupBin, 2);
    setARE(folowupBin, A);
    addTranslation(folowupBin, NULL, trans);
    freeMem(bin, folowupBin);
    free_str_arr(operandParts);
    return;
  case REGISTER_ACCESS:
    operand++; /* skip the r */
    bin = intToBinary(atoi(operand));
    shiftLeft(bin,
              operandNmber == 1 ? offsetFisrtOperand : offsetSecondOperand);
    addTranslation(bin, NULL, trans);
    freeMem(bin, folowupBin);
    return;
  }
}

void addTwoRegisterOperandsToTranslation(char *register1, char *register2,
                                         Translation *trans) {
  char *register1Bin = NULL;
  char *register2Bin = NULL;
  register1Bin = intToBinary(atoi(++register1));
  shiftLeft(register1Bin, 6);
  register2Bin = intToBinary(atoi(++register2));
  shiftLeft(register2Bin, 2);
  orInPlace(register1Bin, register2Bin);
  addTranslation(register1Bin, NULL, trans);
  freeMem(register1Bin, register2Bin);
}

void addTranslationForTwoOperands(char *instructionName, char *srcOperand,
                                  char *destOperand, Label *currentLabel,
                                  Translation *trans, AsmDescriptor *ds) {
  char *mainBin = NULL;
  char *secondaryBin = NULL;
  char *thirdBin = NULL;
  char *label = NULL;
  enum LBA destOperanLlbaId = 0;
  enum LBA srcOperanLlbaId = 0;
  mainBin = intToBinary(getInstructionId(instructionName));
  shiftLeft(mainBin, 6);
  setARE(mainBin, A);
  if (currentLabel != NULL)
    label = cp_string(currentLabel->name);
  destOperanLlbaId = getOperandLBAId(destOperand);
  srcOperanLlbaId = getOperandLBAId(srcOperand);
  secondaryBin = intToBinary(srcOperanLlbaId);
  thirdBin = intToBinary(destOperanLlbaId);
  shiftLeft(secondaryBin, 4);
  shiftLeft(thirdBin, 2);
  orInPlace(mainBin, secondaryBin);
  orInPlace(mainBin, thirdBin);
  addTranslation(mainBin, NULL, trans);
  if (srcOperanLlbaId == REGISTER_ACCESS &&
      destOperanLlbaId == REGISTER_ACCESS) {
    addTwoRegisterOperandsToTranslation(srcOperand, destOperand, trans);
  } else {
    addOperandTranslation(srcOperand, srcOperanLlbaId, trans, 1);
    addOperandTranslation(destOperand, destOperanLlbaId, trans, 2);
  }
  addAsmRowToTable(newAsmRow(1, ds->instructions_tb->translationCounter,
                             ds->line_num, trans, label != NULL, label),
                   ds->instructions_tb);
  freeMem(mainBin, secondaryBin);
  freeMem(label, label);
}

void addTranslationForOneOperand(char *instructionName, OperandLBA *lba,
                                 char *destOperand, Label *currentLabel,
                                 Translation *trans) {
  char *mainBin = NULL;
  char *secondaryBin = NULL;
  char *label = NULL;
  enum LBA lbaId = 0;
  mainBin = intToBinary(getInstructionId(instructionName));
  shiftLeft(mainBin, 6);
  setARE(mainBin, A);
  if (currentLabel != NULL)
    label = cp_string(currentLabel->name);
  lbaId = getOperandLBAId(destOperand);
  secondaryBin = intToBinary(lbaId);
  shiftLeft(secondaryBin, 2);
  orInPlace(mainBin, secondaryBin);
  addTranslation(mainBin, NULL, trans);
  addOperandTranslation(destOperand, lbaId, trans, 2);
  addAsmRowToTable(newAsmRow(1, ds->instructions_tb->translationCounter,
                             ds->line_num, trans, label != NULL, label),
                   ds->instructions_tb);
  freeMem(mainBin, secondaryBin);
  freeMem(label, label);
}

void addInstructionToTranslation(char *instructionName,
                                 int requieredOperandCount,
                                 InstuctionAllowedOpreandLBA *instructionLBA,
                                 StrArr *operand, Label *currentLabel,
                                 Translation *trans, AsmDescriptor *ds) {
  if (requieredOperandCount == 0) {
    addTranslationForZeroOperands(instructionName, currentLabel, trans);
  }
  if (requieredOperandCount == 1) {
    addTranslationForOneOperand(instructionName, instructionLBA->secondOperand,
                                operand->strings[0], currentLabel, trans);
  }
  if (requieredOperandCount == 2) {
    addTranslationForTwoOperands(instructionName, operand->strings[0],
                                 operand->strings[1], currentLabel, trans, ds);
  }
}

void readRestOfInstruction(char *line, char *instructionName,
                           int *lastReadCharIndex, Label *currentLabel,
                           AsmDescriptor *ds) {
  int requieredOperandsCount = 0;
  int validInstructionLine = false;
  char *err = NULL;
  StrArr *operands = NULL;
  Translation *trans = NULL;
  InstuctionAllowedOpreandLBA *instructionLBA = NULL;
  instructionLBA = getAllowedLBAForInstruction(instructionName);
  requieredOperandsCount = getNubmberOfRequieredOperands(instructionName);
  validInstructionLine = getLineValidityForInstruction(
      line + *lastReadCharIndex, requieredOperandsCount, instructionLBA);
  if (!validInstructionLine) {
    err = cat_strings(
        "Error in file ", ds->file_name, " in line ", ds->line_num_string,
        " invalid operands for instuction \"", instructionName, "\"", NULL);
    log_error(ds->err_log, err);
    freeMem(err, err);
    freeInstuctionAllowedOpreandLBA(instructionLBA);
    return;
  }
  trans = newTranslation();
  operands = split(line + *lastReadCharIndex, ",");
  addInstructionToTranslation(instructionName, requieredOperandsCount,
                              instructionLBA, operands, currentLabel, trans,
                              ds);
}

void firstScan(AsmDescriptor *ds) {
  char *word = NULL;
  char *labelName = NULL;
  Label *currentLabel = NULL;
  int lastReadCharIndex = 0;
  char *err = NULL;
  LineFlags *flags;
  while (get_next_line(ds)) {
    flags = newLineFlags();
    clearFlags(flags);
    lastReadCharIndex = 0;
    if (lineEmpyOrComment(ds->line))
      continue;

    word = getNextWordWithWordEndDelimiter(ds->line, &lastReadCharIndex);
    currentLabel = NULL;

    if (isLabelDef(word)) {
      flags->hasLabelDef = true;
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
      /* addLabelToTable(currentLabel, ds->lable_tb); */
    }

    if (isEntryExtern(word)) {
      readEntryExtern(ds->line, word, &lastReadCharIndex, currentLabel);
      freeLabel(currentLabel);
      currentLabel = NULL;
      freeMem(word, labelName);
      continue;
    }

    if (isDataDef(word)) {
      if (flags->hasLabelDef) {
        currentLabel->type = DATA;
        addLabelToTable(currentLabel, ds->lable_tb);
        /* addLabelToTable(currentLabel, ds->lable_tb); */
      }
      readRestOfDataLine(ds->line, word, &lastReadCharIndex, currentLabel);
      freeMem(word, labelName);
      continue;
    }

    if (isInstructionName(word)) {
      if (flags->hasLabelDef) {
        currentLabel->type = INSTRUCTION;
        addLabelToTable(currentLabel, ds->lable_tb);
        /* addLabelToTable(currentLabel, ds->lable_tb); */
      }
      readRestOfInstruction(ds->line, word, &lastReadCharIndex, currentLabel,
                            ds);
      freeMem(word, labelName);
      continue;
    } else {
      err = cat_strings("Error in file ", ds->file_name, " in line ",
                        ds->line_num_string, " invalid operation \"", word,
                        "\"", NULL);

      log_error(ds->err_log, err);
      freeMem(err, err);
    }
    freeMem(word, labelName);
    freeLineFlags(flags);
    flags = NULL;
  }
}
