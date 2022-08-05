
#include "../headers/firstscan.h"

#define N 80
#define MAXLABELNAME 30
#define NUMOFOPCODE 16
#define NUMOFREG 8
#define NUMOFDATACODE 5

char *opcodeTable[NUMOFOPCODE] = {"mov", "cmp", "add", "sub", "not", "clr",
                                  "lea", "inc", "dec", "jmp", "bne", "get",
                                  "prn", "jsr", "rts", "hlt"};
char *regTable[NUMOFREG] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
char *dataCode[NUMOFDATACODE] = {".data", ".struct", ".string", ".entry",
                                 ".extern"};

char label[MAXLABELNAME] = {0};
char prevWord[N] = {0};
int dataCodeNumber =
    -1; /*0 - .data, 1 - .struct, 2 - .string, 3 - .entry, 4 - .extern*/
int opcodeNumber = -1;
char op1[MAXLABELNAME] = {0};
char op2[MAXLABELNAME] = {0};
int countWord = 0;
Translation *trans;

void firstscan() {
  char line[N] = {0};
  AsmRow *row;
  char *err;
  AsmTranslationTable *table;
  while (get_next_line(ds)) {
    trans = newTranslation();
    strcpy(line, ds->line);
    checkLine(line);
    if(opcodeNumber!=-1) checkOpcode();
    if (!correctLabel(label)) {
      err = cat_strings(NULL, "Error in file ", ds->file_name,
                        " invalid name for label: ", label, NULL);
      log_error(ds->err_log, err);
      freeTranslation(trans);
      free(err);
      continue;
    }
    if (dataCodeNumber != -1) { /*checks if data instruction*/
      table = ds->data_tb;
      row = newAsmRow(countWord, table->translationCounter, ds->line_num, trans,
                      !emptyArr(label), label);
    } else {
      table = ds->instructions_tb;
      row = newAsmRow(countWord, table->translationCounter, ds->line_num, trans,
                      !emptyArr(label), label);
    }
    addAsmRowToTable(row, table);
  }
}

void checkLine(char *line) {
  char word[N] = {0};
  char prevChar;
  int i = 0, j = 0;
  while (line[i] != '\0') {

      if (line[i] == ';' && emptyArr(prevWord) && (!isalnum(prevChar))) {
      return;
    }
    if (isspace(line[i])) {
      if ((dataCodeNumber == -1) && (opcodeNumber == -1) && (!emptyArr(word))) {
        addword
      }
      prevChar = line[i];
      i++;
      continue;
    } else {
      if (line[i] == ':') {
        if ((isalnum(prevChar)) && (prevChar != ':') && (emptyArr(label)) && (emptyArr(prevWord)) ){
          word[j] = '\0';
          j = 0;
          strcpy(label, word);
          freeArr(word);
          prevChar = ':';
        } else
          printf("\nError ':' ");
        i++;
        continue;
      }
      if (isdigit(line[i])) {
        if (emptyArr(word) && dataCodeNumber == -1)
          printf("\nError number no data");
        else {
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
        if (prevChar == '.')
          printf("\nError '.' ");
        prevChar = line[i];
        i++;
        continue;
      }
      if (line[i] == ',') {
        if ((!isspace(prevChar) || !emptyArr(word)) && prevChar != ',') {
          addword prevChar = line[i];
        } else
          printf("\nError ',' ");
        i++;
        continue;
      }
      if (!correctChar(line[i]))
        printf("\nError char");
      if (prevChar == ':')
        printf("\nError string");
      else {
        word[j] = line[i];
        j++;
        prevChar = line[i];
      }
      i++;
    }
  }
  if (!emptyArr(word)) {
    addword
  }
  if (prevChar == ',')
    printf("\nError ','");
  freeArr(word);
  return;
}

int emptyArr(const char *arr) {
  if (arr[0] == '\0')
    return true;
  return false;
}

void checkOpcode() {

  if (checkSourceOperand(opcodeNumber, checkTypeOperand(op1))) {
    setSourceOperand(trans->binary[0], checkTypeOperand(op1));
  }
  if (checkDestinationOperand(opcodeNumber, checkTypeOperand(op2))) {
    setDestinationOperand(trans->binary[0], checkTypeOperand(op2));
  }
  if((checkHowOperand(opcodeNumber)==2) && (emptyArr(op1) || emptyArr(op2))) printf("\nNot enough operands");
  if(checkHowOperand(opcodeNumber)==1 && emptyArr(op2)) printf("\nNot enough operands");
  return;
}

void freeArr(char *line) {
  int i;
  for (i = 0; i < N; i++) {
    line[i] = '\0';
  }
}

enum WORD_TYPE srchWord(char *arr) {
  if (isOpcode(arr) != -1)
    return ISOPCODE;
  if (isRegistr(arr) != -1)
    return ISREGISTR;
  if (isData(arr) != -1)
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
  return -1;
}

int isRegistr(char *arr) {
  int i;
  for (i = 0; i < NUMOFREG; i++) {
    if (strcmp(arr, regTable[i]) == 0)
      return i;
  }
  return -1;
}

int isData(char *arr) {
  int i;
  for (i = 0; i < NUMOFDATACODE; i++) {
    if (strcmp(arr, dataCode[i]) == 0)
      return i;
  }
  return -1;
}

int isNumber(char *arr) {
  char buf[30] = {0};
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
    strcpy(op2, ".\0");
    strcat(op2, buf);
  } else {
    strcpy(buf, op1);
    strcpy(op1, ".\0");
    strcat(op1, buf);
  }
  return;
}

int checkTypeOperand(char *operand) {
  if (isRegistr(operand)!=-1)
    return 3;
  if (operand[0] == '.')
    return 2;
  if (operand[0] == '#')
    return 0;
  return 1;
}

int checkNumberArr(char *arr) {
  int i = 0;
  if (arr[i] == '-' || arr[i] == '+') {
    if (arr[i + 1] == '\0') {
      printf("Error number incorrect");
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

/* returns the length of the string, not including the "", -1 for invalid string
 */
int addString(char *str) {
  int current = 0;
  int last = strlen(str) - 1;
  if (str[current++] != '\"' || str[last] != '\"')
    return -1;
  str[last] = '\0';
  while (str[current] != '\0') {
    if (str[current] == '\"')
      return -1;
    addTranslation(aToBin(str[current]), NULL, trans);
    current++;
  }
  addTranslation(aToBin('\0'), NULL, trans);
  return current - 2;
}

void addOperand(char *word) {
  if (emptyArr(op1) && checkHowOperand(opcodeNumber) >= 1) {
    if (emptyArr(op2) && (checkHowOperand(opcodeNumber) == 1))
      strcpy(op2, word);
    else {if (checkHowOperand(opcodeNumber)>1) strcpy(op1, word);}

  } else {
    if (emptyArr(op2) && checkHowOperand(opcodeNumber) == 2) {
      strcpy(op2, word);
    } else
      printf(" \nOperand error");
  }
}

int checkHowOperand(const int opcode) {

  if ((opcode >= 0 && opcode <= 3) || opcode == 6){
    return 2;}
  if (opcode >= 4 && opcode <= 13){
    return 1;}
  if (opcode >= 14 && opcode <= 15){
    return 0;}
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
      (isData(word) == -1) && (strcmp(word, "psw") != 0))
    return true;
  return false;
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
  printf("Operand destination error");
  return;
}

void setSecondRegistr(char *bin, char *reg) {
  char buf[WORD_SIZE];
  int i;
  if (emptyArr(reg))
    return;
  strcpy(buf, intToBinary(isRegistr(reg)));
  for (i = 1; i < 4; i++) {
    bin[WORD_SIZE - (i + 2)] = buf[WORD_SIZE - i];
  }
  return;
}

void setSourceOperand(char *bin, int n) {
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
  printf("Operand source error");
  return;
}

void checkWord(char *word) {
  enum WORD_TYPE wordType;
  char bin[WORD_SIZE];
  int isReadingString = false;
  int binTransLen=0;
  char *err;
  const int INVALID = -1;
  wordType = srchWord(word);
  switch (wordType) {

  case ISDATA: /*.data/.string/.struct/.entry/.extren*/

    dataCodeNumber = isData(word);
    countWord--;


    break;

  case ISNUMBER:

    if (word[0] == '.' || word[0] == '#') {
      if (isNumber(prevWord)) {
        printf("\nError number: just natural numbers");
        break;
      }
      if (word[0] == '#')
        addOperand(word);
      if (word[0] == '.')
        addPointOperand();
      strcpy(bin, intToBinary(atoi(word + 1)));
      shiftLeft(bin, 2);
      addTranslation(bin, NULL, trans);

    } else {
      if (dataCodeNumber == 0 || dataCodeNumber == 1) {
        strcpy(bin, intToBinary(atoi(word)));
        if (dataCodeNumber == 1) {
          if (emptyArr(op1)) {
            strcpy(op1, word);
          } else
            printf("\nError struct");
        }

        addTranslation(bin, NULL, trans);
      } else
        printf("\nError code");
    }

    break;

  case ISOPCODE:

    if (opcodeNumber == -1) {
      opcodeNumber = isOpcode(word);
      strcpy(bin, intToBinary(opcodeNumber));
      shiftLeft(bin, 6);
      addTranslation(bin, NULL, trans);
    } else
      printf("\nOpcode error");
    break;

  case ISREGISTR:

    addOperand(word);
    if (!emptyArr(op1) && emptyArr(op2)) {
      strcpy(bin, intToBinary(isRegistr(word)));
      shiftLeft(bin, 6);
      addTranslation(bin, NULL, trans);
    }
    if (!emptyArr(op1) && !emptyArr(op2)) {
      if (isRegistr(op1) != -1) {
        setSecondRegistr(trans->binary[countWord - 1], word);
      } else {
        strcpy(bin, intToBinary(isRegistr(word)));
        shiftLeft(bin, 2);
        addTranslation(bin, NULL, trans);
      }
    }
    break;



  case UNKNOWN: /* reading body of .string or .entry or .extern or .struct or
                 body of operation (label of operation valid or invalid)
                 */
    addOperand(word);
    isReadingString = (dataCodeNumber == 1 && !emptyArr(op2)) ||
                      (dataCodeNumber == 2 && !emptyArr(op1));
    if (!isReadingString) {
      addTranslation(NULL, label, trans);
      break;
    }

    if (dataCodeNumber == 1)
      binTransLen = addString(op2);
    if (dataCodeNumber == 2)
      binTransLen = addString(op1);
    if (binTransLen == INVALID) {
      err = cat_strings(NULL, "Error in file ", ds->file_name, " in line ",
                        ds->line_num, "invalid string definition", NULL);
      log_error(ds->err_log, err);
      free(err);
    }
    countWord += binTransLen;
    break;
  }
}
