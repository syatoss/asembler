
#include "../headers/firstscan.h"

#define N 255
#define MAXLABELNAME 30
#define NUMOFOPCODE 16
#define NUMOFREG 8
#define NUMOFDATACODE 5
#define INVALID -1
#define EMPTY -2

char *opcodeTable[NUMOFOPCODE] = {"mov", "cmp", "add", "sub", "not", "clr",
                                  "lea", "inc", "dec", "jmp", "bne", "get",
                                  "prn", "jsr", "rts", "hlt"};
char *regTable[NUMOFREG] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
char *dataCode[NUMOFDATACODE] = {".data", ".struct", ".string", ".entry",
                                 ".extern"};

char label[MAXLABELNAME] = {0};
char prevWord[MAXLABELNAME] = {0};
int dataCodeNumber =
    -1; /*0 - .data, 1 - .struct, 2 - .string, 3 - .entry, 4 - .extern*/
int opcodeNumber = -1;
char op1[MAXLABELNAME] = {0};
char op2[MAXLABELNAME] = {0};
int countWord = 0;
Translation *trans;

void clearPrevValues() {
  freeArr(op1);
  freeArr(op2);
  freeArr(label);
  freeArr(prevWord);
  dataCodeNumber = INVALID;
  opcodeNumber = INVALID;
  countWord = 0;
  trans = NULL;
}

void firstscan() {
  char line[N + 1] = {0};
  AsmRow *row;
  char *err;
  AsmTranslationTable *table;
  while (get_next_line(ds)) {
    clearPrevValues();
    trans = newTranslation();
    strcpy(line, ds->line);
    checkLine(line);
    if (opcodeNumber != INVALID)
      checkOpcode();
    if (!correctLabel(label)) {
      err = cat_strings(NULL, "Error in file ", ds->file_name,
                        " invalid name for label: ", label, NULL);
      log_error(ds->err_log, err);
      freeTranslation(trans);
      free(err);
      continue;
    }
    if (dataCodeNumber != INVALID) { /*checks if data instruction*/
      table = ds->data_tb;
      checkData();

      if (!emptyArr(label) && dataCodeNumber != ENTRY &&
          dataCodeNumber != EXTERN) {
        addLabelToTable(newLabel(label, ds->line_num, NONE, DATA),
                        ds->lable_tb);
        if (dataCodeNumber != DATAWORD)
          countWord--;
      }
      row = newAsmRow(countWord, table->translationCounter, ds->line_num, trans,
                      !emptyArr(label), label);

      addAsmRowToTable(row, table);
    }
    if (opcodeNumber != INVALID) {
      table = ds->instructions_tb;
      if (!emptyArr(label)) {
        addLabelToTable(newLabel(label, ds->line_num, NONE, INSTRUCTION),
                        ds->lable_tb);
      }
      row = newAsmRow(countWord, table->translationCounter, ds->line_num, trans,
                      !emptyArr(label), label);

      addAsmRowToTable(row, table);
    }
    if ((opcodeNumber == INVALID) && (dataCodeNumber == INVALID) &&
        (!emptyArr(label))) {
      err = cat_strings("Error in file ", ds->file_name, " in line ",
                        ds->line_num_string, " missing instruction or data",
                        NULL);
      log_error(ds->err_log, err);
      free(err);
    }
    /* addAsmRowToTable(row, table);*/
  }
}

void checkLine(char *line) {
  char word[MAXLABELNAME] = {0};
  char prevChar;
  char *pivot;
  char *err = NULL;
  char singleChar[2] = {'\0', '\0'};
  int i = 0, j = 0;
  while (line[i] != '\0') {

    if (line[i] == ';' && emptyArr(prevWord) && (!isalnum(prevChar))) {
      return;
    }
    if (isspace(line[i])) {
      /* add the word if no data or instruction has been read */
      if ((dataCodeNumber == INVALID) && (opcodeNumber == INVALID) &&
          (!emptyArr(word))) {
        addword
      }
      if (word[0] == '\"') {
        word[j] = line[i];
        j++;
      }
      prevChar = line[i];
      i++;
      continue;
    } else {
      if (line[i] == ':') {
        if ((isalnum(prevChar)) && (prevChar != ':') && (emptyArr(label)) &&
            (emptyArr(prevWord))) {
          word[j] = '\0';
          j = 0;
          strcpy(label, word);
          freeArr(word);
          prevChar = ':';

        } else {
          err = cat_strings("Error in file ", ds->file_name, " in line ",
                            ds->line_num_string, " invalid char \':\'", NULL);
          log_error(ds->err_log, err);
          free(err);
          err = NULL;
        }
        i++;
        continue;
      }
      if (isdigit(line[i])) {
        if (emptyArr(word) && dataCodeNumber == INVALID) {
          err =
              cat_strings("Error in file ", ds->file_name, " in line ",
                          ds->line_num_string, " invalid use of number ", NULL);
          log_error(ds->err_log, err);
          free(err);
          err = NULL;

        } else {
          word[j] = line[i];
          j++;
        }
        prevChar = line[i];
        i++;
        continue;
      }
      if (line[i] == '.') {
        if (!emptyArr(word)) {
          addword
        }
        if (emptyArr(word)) {
          word[j] = line[i];
          j++;
        }
        if (prevChar == '.' || prevChar == ':') {

          err = cat_strings("Error in file ", ds->file_name, " in line ",
                            ds->line_num_string, " invalid use of char \'.\' ",
                            NULL);
          log_error(ds->err_log, err);
          free(err);
          err = NULL;
        }
        prevChar = line[i];
        i++;
        continue;
      }
      if (line[i] == ',') {
        if ((!isspace(prevChar) || !emptyArr(word)) && prevChar != ',') {
          addword prevChar = line[i];
        } else {
          err = cat_strings("Error in file ", ds->file_name, " in line ",
                            ds->line_num_string, " invalid use of char \',\' ",
                            NULL);
          log_error(ds->err_log, err);
          free(err);
          err = NULL;
        }
        i++;
        continue;
      }
      if (!correctChar(line[i])) {
        singleChar[0] = line[i];
        err = cat_strings("Error in file ", ds->file_name, " in line ",
                          ds->line_num_string, " invalid use of char \'",
                          singleChar, "\'", NULL);
        log_error(ds->err_log, err);
        free(err);
        err = NULL;
      }
      if (prevChar == ':') {
        err = cat_strings("Error in file ", ds->file_name, " in line ",
                          ds->line_num_string, " invalid use of char \',\' ",
                          NULL);
        log_error(ds->err_log, err);
        free(err);
        err = NULL;
      } else {
        word[j] = line[i];
        j++;
        prevChar = line[i];
      }
      i++;
    }
  }
  if (!emptyArr(word)) {
    pivot = trim(word);
    strcpy(word, pivot);
    addword
  }
  if (prevChar == ',') {
    err = cat_strings("Error in file ", ds->file_name, " in line ",
                      ds->line_num_string, " trailing \',\' ", NULL);
    log_error(ds->err_log, err);
    free(err);
    err = NULL;
  }
  freeArr(word);
  return;
}

int emptyArr(const char *arr) {
  if (arr[0] == '\0')
    return true;
  return false;
}
void checkData() {
  char *err;
  if (dataCodeNumber == STRUCT && (emptyArr(op1) || emptyArr(op2))) {
    err = cat_strings("Error in file ", ds->file_name, " in line ",
                      ds->line_num_string, " struct definition missing data ",
                      NULL);
    log_error(ds->err_log, err);
    free(err);
    err = NULL;
  }
  if (dataCodeNumber == STRING && emptyArr(op1)) {
    err = cat_strings("Error in file ", ds->file_name, " in line ",
                      ds->line_num_string, " string definition missing data ",
                      NULL);
    log_error(ds->err_log, err);
    free(err);
    err = NULL;
  }
  if (dataCodeNumber == EXTERN) {

    if (!emptyArr(op1) && !emptyArr(op2)) {
      err = cat_strings("Error in file ", ds->file_name, " in line ",
                        ds->line_num_string, " invalid use of extern ", NULL);
      log_error(ds->err_log, err);
      free(err);
      err = NULL;
    } else {

      addLabelToTable(newLabel(op1, ds->line_num, EXTERNAL, DATA),
                      ds->lable_tb);
    }
  }
}
void checkOpcode() {
  char *err;

  if (checkSourceOperand(opcodeNumber, checkTypeOperand(op1))) {
    setSourceOperand(trans->binary[0], checkTypeOperand(op1));
  } else if (!emptyArr(op1)) {
    err = cat_strings("Error in file ", ds->file_name, " in line ",
                      ds->line_num_string, " invalid source operand ", NULL);
    log_error(ds->err_log, err);
    free(err);
    err = NULL;
  }
  if (checkDestinationOperand(opcodeNumber, checkTypeOperand(op2))) {
    setDestinationOperand(trans->binary[0], checkTypeOperand(op2));
  } else if (!emptyArr(op2)) {
    err =
        cat_strings("Error in file ", ds->file_name, " in line ",
                    ds->line_num_string, " invalid destination operand ", NULL);
    log_error(ds->err_log, err);
    free(err);
    err = NULL;
  }
  if ((checkHowOperand(opcodeNumber) == 2) &&
      (emptyArr(op1) || emptyArr(op2))) {
    err =
        cat_strings("Error in file ", ds->file_name, " in line ",
                    ds->line_num_string, " invalid number of operands ", NULL);
    log_error(ds->err_log, err);
    free(err);
    err = NULL;
  }
  if ((checkHowOperand(opcodeNumber) == 1) && (emptyArr(op2))) {
    err =
        cat_strings("Error in file ", ds->file_name, " in line ",
                    ds->line_num_string, " invalid number of operands ", NULL);
    log_error(ds->err_log, err);
    free(err);
    err = NULL;
  }
  return;
}

void freeArr(char *line) {
  int i;
  int size = strlen(line);
  for (i = 0; i < size; i++) {
    line[i] = '\0';
  }
}

enum WORD_TYPE srchWord(char *arr) {
  if (isOpcode(arr) != INVALID)
    return ISOPCODE;
  if (isRegistr(arr) != INVALID)
    return ISREGISTR;
  if (isData(arr) != INVALID)
    return ISDATA;
  if (isNumber(arr))
    return ISNUMBER;
  return UNKNOWN;
}

int isOpcode(char *arr) {
  int i;
  for (i = 0; i < NUMOFOPCODE; i++) {
    if (strcmp(arr, opcodeTable[i]) == 0)
      return i;
  }
  return INVALID;
}

int isRegistr(char *arr) {
  int i;
  for (i = 0; i < NUMOFREG; i++) {
    if (strcmp(arr, regTable[i]) == 0)
      return i;
  }
  return INVALID;
}

int isData(char *arr) {
  int i;
  for (i = 0; i < NUMOFDATACODE; i++) {
    if (strcmp(arr, dataCode[i]) == 0)
      return i;
  }
  return INVALID;
}

int isNumber(char *arr) {
  char buf[MAXLABELNAME] = {0};
  strcpy(buf, arr);
  if (arr[0] == '.' || arr[0] == '#') {
    strcpy(buf, arr + 1);
  }
  if (checkNumberArr(buf))
    return true;
  return false;
}
void addPointOperand() {
  char buf[MAXLABELNAME] = {0};
  if (!emptyArr(op2)) {
    strcpy(buf, op2);
    strcpy(op2, ".");
    strcat(op2, buf);
  } else {
    strcpy(buf, op1);
    strcpy(op1, ".");
    strcat(op1, buf);
  }
  return;
}

int checkTypeOperand(char *operand) {
  if (isRegistr(operand) != INVALID)
    return 3;
  if (operand[0] == '.')
    return 2;
  if (operand[0] == '#')
    return 0;
  return 1;
}

int checkNumberArr(char *arr) {
  int i = 0;
  char *err;
  if (arr[i] == '-' || arr[i] == '+') {
    if (arr[i + 1] == '\0') {
      err = cat_strings("Error in file ", ds->file_name, " in line ",
                        ds->line_num_string, " invalid number ", NULL);
      log_error(ds->err_log, err);
      free(err);
      err = NULL;
      return false;
    }
    i++;
  }
  while (arr[i] != '\0') {

    if (!isdigit(arr[i]))
      return false;
    i++;
  }
  return true;
}

/* returns the length of the string, not including the "", -1 for invalid
 * string
 */
int addString(char *str) {
  int current = 0;
  int last = strlen(str) - 1;
  if (str[current++] != '\"' || str[last] != '\"')
    return INVALID;
  str[last] = '\0';
  while (str[current] != '\0') {
    if (str[current] == '\"')
      return INVALID;
    addTranslation(aToBin(str[current]), NULL, trans);
    current++;
  }
  addTranslation(aToBin('\0'), NULL, trans);
  if (current == 1)
    return EMPTY;
  return current - 1;
}
void addOperand(char *word) {
  char *err;
  if (emptyArr(op2) && checkHowOperand(opcodeNumber) == 1) {
    strcpy(op2, word);
    if (checkTypeOperand(op2) == 1 && dataCodeNumber == INVALID)
      addTranslation(NULL, op2, trans);
    return;
  }
  if (emptyArr(op1) && checkHowOperand(opcodeNumber) == 2) {
    strcpy(op1, word);
    if (checkTypeOperand(op1) == 1 && dataCodeNumber == INVALID)
      addTranslation(NULL, op1, trans);
    return;
  }
  if (emptyArr(op2) && checkHowOperand(opcodeNumber) == 2) {
    strcpy(op2, word);
    if (checkTypeOperand(op2) == 1 && dataCodeNumber == INVALID)
      addTranslation(NULL, op2, trans);
    return;
  }
  err = cat_strings("Error in file ", ds->file_name, " in line ",
                    ds->line_num_string, " invalid operand ", NULL);
  log_error(ds->err_log, err);
  free(err);
  err = NULL;
}

int checkHowOperand(const int opcode) {

  if ((opcode >= 0 && opcode <= 3) || opcode == 6) {
    return 2;
  }
  if (opcode >= 4 && opcode <= 13) {
    return 1;
  }
  if (opcode >= 14 && opcode <= 15) {
    return 0;
  }
  return 2;
}

int correctChar(char ch) {
  if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
        (ch >= '0' && ch <= '9') || (ch == '\"') || (ch == '-') ||
        (ch == '#') || (ch == '+')))
    return false;
  return true;
}

int correctLabel(char *word) {

  if ((isOpcode(word) == -1) && (isRegistr(word) == -1) &&
      (isData(word) == -1) && (strcmp(word, "psw") != 0) &&
      correctLabelWord(word))
    return true;
  return false;
}
int correctLabelWord(char *word) {
  int i = 0;
  while (word[i] != '\0') {
    if (!isalnum(word[i]))
      return false;
    i++;
  }
  return true;
}
int checkDestinationOperand(int opcode, int type) {
  if (checkHowOperand(opcode) >= 1 && type >= 1)
    return true;
  if ((opcode == isOpcode("cmp\0") || opcode == isOpcode("prn\0")) && type == 0)
    return true;
  return false;
}

int checkSourceOperand(int opcode, int type) {
  if ((checkHowOperand(opcode) == 2) && (opcode != isOpcode("lea\0")))
    return true;
  if ((opcode == isOpcode("lea\0")) && (type == 1 || type == 2))
    return true;

  return false;
}

void setDestinationOperand(char *bin, int n) {
  char *err;
  if (n == 0) {
    bin[6] = '0';
    bin[7] = '0';
    return;
  }
  if (n == 1) {
    bin[6] = '0';
    bin[7] = '1';
    return;
  }
  if (n == 2) {
    bin[6] = '1';
    bin[7] = '0';
    return;
  }
  if (n == 3) {
    bin[6] = '1';
    bin[7] = '1';
    return;
  }
  err = cat_strings("Error in file ", ds->file_name, " in line ",
                    ds->line_num_string, " invalid destination operand ", NULL);
  log_error(ds->err_log, err);
  free(err);
  err = NULL;
  return;
}

void setSecondRegistr(char *bin, char *reg) {
  char buf[WORD_SIZE + 1] = {0};
  int i;
  if (emptyArr(reg))
    return;
  strcpy(buf, intToBinary(isRegistr(reg)));
  shiftLeft(buf, 2);
  for (i = 4; i < 8; i++) {
    bin[i] = buf[i];
  }
}

void setSourceOperand(char *bin, int n) {
  char *err;
  if (n == 0) {
    bin[4] = '0';
    bin[5] = '0';
    return;
  }
  if (n == 1) {
    bin[4] = '0';
    bin[5] = '1';
    return;
  }
  if (n == 2) {
    bin[4] = '1';
    bin[5] = '0';
    return;
  }
  if (n == 3) {
    bin[4] = '1';
    bin[5] = '1';
    return;
  }
  err = cat_strings("Error in file ", ds->file_name, " in line ",
                    ds->line_num_string, " invalid source operand ", NULL);
  log_error(ds->err_log, err);
  free(err);
  err = NULL;
  return;
}

void checkWord(char *word) {
  enum WORD_TYPE wordType;
  char bin[WORD_SIZE + 1];
  int isReadingString = false;
  int binTransLen = 0;
  char *err;
  char *pivot;
  wordType = srchWord(word);
  switch (wordType) {

  case ISDATA: /*.data/.string/.struct/.entry/.extren*/

    dataCodeNumber = isData(word);
    if (dataCodeNumber == DATAWORD)
      countWord--;
    break;

  case ISNUMBER:

    if (word[0] == '.' || word[0] == '#') {
      if (isNumber(prevWord)) {
        err = cat_strings("Error in file ", ds->file_name, " in line ",
                          ds->line_num_string,
                          " float point numbers not allowed ", NULL);
        log_error(ds->err_log, err);
        free(err);
        err = NULL;
        break;
      }
      if (word[0] == '#')
        addOperand(word);
      if (word[0] == '.')
        addPointOperand();
      pivot = intToBinary(atoi(word + 1));
      strcpy(bin, pivot);
      free(pivot);
      shiftLeft(bin, 2);
      addTranslation(bin, NULL, trans);
      break;
    }
    if (dataCodeNumber == DATAWORD || dataCodeNumber == STRUCT) {
      pivot = intToBinary(atoi(word));
      strcpy(bin, pivot);
      free(pivot);
      if (dataCodeNumber == STRUCT) {
        if (emptyArr(op1)) {
          strcpy(op1, word);
        } else {
          err = cat_strings("Error in file ", ds->file_name, " in line ",
                            ds->line_num_string, " invalid struct definition ",
                            NULL);
          log_error(ds->err_log, err);
          free(err);
          err = NULL;
        }
      }

      addTranslation(bin, NULL, trans);
    }

    break;

  case ISOPCODE:

    if (opcodeNumber == INVALID) {
      opcodeNumber = isOpcode(word);
      pivot = intToBinary(opcodeNumber);
      strcpy(bin, pivot);
      free(pivot);
      shiftLeft(bin, 6);
      addTranslation(bin, NULL, trans);
    } else {
      err = cat_strings("Error in file ", ds->file_name, " in line ",
                        ds->line_num_string, " duplicate instruction ", NULL);
      log_error(ds->err_log, err);
      free(err);
      err = NULL;
    }
    break;

  case ISREGISTR:

    addOperand(word);
    if (!emptyArr(op1) && emptyArr(op2)) {
      pivot = intToBinary(isRegistr(word));
      strcpy(bin, pivot);
      free(pivot);
      shiftLeft(bin, 6);
      addTranslation(bin, NULL, trans);
    }
    if (!emptyArr(op1) && !emptyArr(op2)) {
      if (isRegistr(op1) != INVALID) {
        setSecondRegistr(trans->binary[countWord - 2], word);
        countWord--;
      } else {
        pivot = intToBinary(isRegistr(word));
        strcpy(bin, pivot);
        shiftLeft(bin, 2);
        addTranslation(bin, NULL, trans);
        free(pivot);
      }
    }
    break;

  case UNKNOWN: /* reading body of .string or .entry or .extern or .struct or
           body of operation (label of operation valid or invalid)
           */
    addOperand(word);
    isReadingString = (dataCodeNumber == STRUCT && !emptyArr(op2)) ||
                      (dataCodeNumber == STRING && !emptyArr(op1));
    if (!isReadingString) {
      break;
    }

    if (dataCodeNumber == STRUCT)
      binTransLen = addString(op2);
    if (dataCodeNumber == STRING)
      binTransLen = addString(op1);
    if (binTransLen == EMPTY)
      countWord++;
    if (binTransLen == INVALID) {
      err =
          cat_strings(NULL, "Error in file ", ds->file_name, " in line ",
                      ds->line_num_string, " invalid string definition", NULL);
      log_error(ds->err_log, err);
      free(err);
    }
    countWord += binTransLen;
    break;
  }
}
