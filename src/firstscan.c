
#include "../headers/firstscan.h"


#define N 80
#define MAXLABELNAME 30
#define NUMOFOPCODE 16
#define NUMOFREG 8
#define NUMOFDATACODE 5

#define addword \
    word[j] = '\0';\
    j = 0;\
    countWord++;\
    checkWord(word);\
    strcpy(prevWord, word);\
    freeArr(word);


enum WORD_TYPE { ISDATA, ISOPCODE, ISREGISTR, ISNUMBER, UNKNOWN };


char *opcodeTable[NUMOFOPCODE]={"mov\0","cmp\0","add\0","sub\0","not\0","clr\0", "lea\0", "inc\0","dec\0",
                                "jmp\0", "bne\0", "get\0", "prn\0", "jsr\0", "rts\0", "hlt\0"};
char *regTable[NUMOFREG]={"r0\0","r1\0","r2\0","r3\0","r4\0","r5\0","r6\0","r7\0"};
char *dataCode[NUMOFDATACODE]={".data\0",".struct\0",".string\0",".entry\0", ".extern\0"};


char label[MAXLABELNAME]={0};
char prevWord[N]={};
int dataCodeNumber=-1;
int opcodeNumber=-1;
char op1[MAXLABELNAME]={};
char op2[MAXLABELNAME]={};
int countWord=0;

int firstscan () {
    char line[N]={};
    Translation *trans = newTranslation();
    AsmRow *row;
    while(get_next_line(*ds))
    {
        strcpy(line, ds->line);
        checkLine(line);
    }
//    if(opcodeNumber!=-1) checkOpcode(trans->binary[0]);
    if(!correctLabel(label)) printf("The label name is incorrect");
    if(dataCodeNumber!=-1) newAsmRow(countWord,dc,trans, !emptyArr(label)),label);
    if(opcodeNumber!=-1) newAsmRow(countWord,ic, trans, !emptyArr(label),label);
    addAsmRowToTable(row,table);

}

void checkLine(char *line) {
    char word[N] = {};
    char prevChar;
    int i = 0, j = 0;
    while (line[i] != '\0') {
        if (line[i] == ';' && i == 0) {
            return;
        }
        if (isspace(line[i])) {
            if (dataCodeNumber == -1 && opcodeNumber == -1 && !emptyArr(word)) {
                addword
            }
            prevChar = line[i];
            i++;
            continue;
        } else {
            if (line[i] == ':') {
                if (isalnum(prevChar) && prevChar != ':' && emptyArr(label)) {
                    word[j] = '\0';
                    j = 0;
                    strcpy(label, word);
                    freeArr(word);
                    prevChar = ':';
                } else printf("\nError ':' ");
                i++;
                continue;
            }
            if (isdigit(line[i])) {
                if (emptyArr(word) && dataCodeNumber == -1) printf("\nError number no data");
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
                if (prevChar == '.') printf("\nError '.' ");
                prevChar = line[i];
                i++;
                continue;
            }
            if (line[i] == ',') {
                if ((!isspace(prevChar) || !emptyArr(word)) && prevChar != ',') {
                    addword
                    prevChar = line[i];
                } else printf("\nError ',' ");
                i++;
                continue;
            }
            if (!correctChar(line[i])) printf("\nError char");
            if (prevChar == ':') printf("\nError string");
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
    if (prevChar == ',') printf("\nError ','");
    freeArr(word);
    return;
}

int emptyArr(const char *arr) {
    if (arr[0] == '\0') return true;
    return false;
}
void checkOpcode(char* bin, Translation* trans)
{

    if(checkSourceOperand(opcodeNumber, checkTypeOperand(op1))) {
        setSourceOperand(trans->binary[0], checkTypeOperand(op1) );
    }
    if(checkDestinationOperand(opcodeNumber, checkTypeOperand(op2))){
        setDestinationOperand(trans->binary[0], checkTypeOperand(op2));
    }
    return;
}
void freeArr(char *line) {
    int i;
    for (i = 0; i < N; i++) {
        line[i] = '\0';
    }
}

enum WORD_TYPE srchWord(char *arr) {
    if (isOpcode(arr) != -1) return ISOPCODE;
    if (isRegistr(arr) != -1) return ISREGISTR;
    if (isData(arr) != -1) return ISDATA;
    if (isNumber(arr)) return ISNUMBER;
    return UNKNOWN;
}

int isOpcode(char *arr) {
    int i;
    for (i = 0; i < NUMOFOPCODE; i++) {
        if (strcmp(arr, opcodeTable[i]) == 0) return i;
    }
    return -1;
}

int isRegistr(char *arr) {
    int i;
    for (i = 0; i < NUMOFREG; i++) {
        if (strcmp(arr, regTable[i]) == 0) return i;
    }
    return -1;
}

int isData(char *arr) {
    int i;
    for (i = 0; i < NUMOFDATACODE; i++) {
        if (strcmp(arr, dataCode[i]) == 0) return i;
    }
    return -1;
}

int isNumber(char *arr) {
    char buf[30] = {};
    strcpy(buf, arr);
    if (arr[0] == '.' || arr[0] == '#') {
        strcpy(buf, arr + 1);

    }
    if (checkNumberArr(buf)) return true;
    return false;
}
void addPointOperand()
{
    char buf[MAXLABELNAME]={};
    if(!emptyArr(op2)) {
        strcpy(buf, op2);
        strcpy(op2, ".\0");
        strcat(op2,buf);
    } else {
        strcpy(buf, op1);
        strcpy(op1, ".\0");
        strcat(op1, buf);
    }
    return;
}
int checkTypeOperand(char* operand)
{
    if(isRegistr(operand)) return 3;
    if(operand[0]=='.') return 2;
    if(operand[0]=='#') return 0;
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

        if (!isdigit(arr[i])) return false;
        i++;
    }
    return true;
}

int addString(const char* str) {
    int i = 0;
    if (str[i] != '\"') return -1;
    while (str[i] != '\0') {
        i++;
        if (str[i] == '\"') {
            addTranslation(aToBin('\0'), NULL, trans);
            return i - 2;
        }
        addTranslation(aToBin(str[i]), NULL, trans);
    }
    return i - 2;
}

void addOperand(char *word) {
    if (emptyArr(op1) && checkHowOperand(opcodeNumber)>=1) {
        if(emptyArr(op2) && checkHowOperand(opcodeNumber)==1) strcpy(op2, word);
        else strcpy(op1, word);

    } else {
        if (emptyArr(op2) && checkHowOperand(opcodeNumber)==2) {
            strcpy(op2, word);
        } else printf(" \nOperand error");
    }
}

int checkHowOperand(const int opcode)
{

    if((opcode>=0 && opcode<=3) || opcode==6) return 2;
    if(opcode>=4 && opcode<=13) return 1;
    if(opcode>=14 && opcode<=15) return 0;
    return 2;
}

int correctChar(char ch) {
    if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || (ch == '\"') ||
          (ch == '-') || (ch == '#') || (ch == '+')))
        return false;
    return true;
}
int correctLabel(char* word)
{
    if((isOpcode(word) == -1) && (isRegistr(word) == -1) && (isData(word) == -1) && (strcmp(word, "psw")!=0)) return true;
    return false;

}
int checkDestinationOperand(int opcode, int type)
{
    if(checkHowOperand(opcode)>=1 && type>=1) return true;
    if((opcode== isOpcode("cmp\0") || opcode== isOpcode("prn\0")) && type == 0 ) return true;
    return false;

}
int checkSourceOperand(int opcode, int type)
{
    if((checkHowOperand(opcode)==2) && (opcode!= isOpcode("lea\0"))) return true;
    if((opcode== isOpcode("lea\0") ) && (type == 1 || type == 2)) return true;
    return false;

}
void setDestinationOperand(char* bin, int n)
{
    if(n==0) {bin[4]='0'; bin[5]='0'; return;}
    if(n==1) {bin[4]='0'; bin[5]='1'; return;}
    if(n==2) {bin[4]='1'; bin[5]='0'; return;}
    if(n==3) {bin[4]='1'; bin[5]='1'; return;}
    printf("Operand destination error");
    return;
}
void setSecondRegistr(char* bin, char* reg)
{
    char* buf;
    int i;
    if(emptyArr(reg)) return;
    strcpy(buf, intToBinary(isRegistr(reg)));
    for(i=1; i<4; i++)
    {
        bin[WORD_SIZE-(i+2)]=buf[WORD_SIZE-i];
    }
    return;
}
void setSourceOperand(char* bin, int n)
{
    if(n==0) {bin[6]='0'; bin[7]='0'; return;}
    if(n==1) {bin[6]='0'; bin[7]='1'; return;}
    if(n==2) {bin[6]='1'; bin[7]='0'; return;}
    if(n==3) {bin[6]='1'; bin[7]='1'; return;}
    printf("Operand source error");
    return;

}
void checkWord(char *word) {
    enum WORD_TYPE wordType;
    char *bin[WORD_SIZE];
    wordType = srchWord(word);
    switch (wordType) {

        case ISDATA:

            dataCodeNumber = isData(word);

            break;

        case ISNUMBER:

            if (word[0] == '.' || word[0] == '#') {
                if (isNumber(prevWord)) {
                    printf("\nError number: just natural numbers");
                    break;
                }
                if (word[0] == '#') addOperand(word);
                if(word[0] == '.') addPointOperand();
                strcpy(*bin, intToBinary(atoi(word + 1)));
                shiftLeft(*bin, 2);
                addTranslation(*bin, NULL, trans);

            } else {
                if (dataCodeNumber == 0 || dataCodeNumber == 1) {
                    strcpy(*bin, intToBinary(atoi(word)));
                    if (dataCodeNumber == 1) {
                        if (emptyArr(op1)) {
                            strcpy(op1, word);
                        } else printf("\nError struct");
                    }

                    addTranslation(*bin, NULL, trans);
                } else printf("\nError code");
            }

            break;

        case ISOPCODE:

            if (opcodeNumber == -1) {
                opcodeNumber = isOpcode(word);
                strcpy(*bin, intToBinary(opcodeNumber));
                shiftLeft(*bin,6);
                addTranslation(*bin, NULL, trans);
            } else printf("\nOpcode error");
            break;

        case ISREGISTR:

            addOperand(word);
            if (!emptyArr(op1) && emptyArr(op2)) {
                strcpy(*bin, intToBinary(isRegistr(word)));
                shiftLeft(*bin,6);
                addTranslation(*bin, NULL, trans);
            }
            if (!emptyArr(op1) && !emptyArr(op2)) {
                if (isRegistr(op1) != -1) {
                    setSecondRegistr(trans->binary[countWord-1], word);
                }
                else {
                    strcpy(*bin, intToBinary(isRegistr(word)));
                    shiftLeft(*bin,2);
                    addTranslation(*bin, NULL, trans);
                }
            }
            break;

        case UNKNOWN:

            addOperand(word);
            int i;
            if ((dataCodeNumber == 1 && !emptyArr(op2)) || (dataCodeNumber == 2 && !emptyArr(op1))) {
                if (dataCodeNumber == 1) i = addString(op2);
                else (dataCodeNumber == 2) i = addString(op1);
                if (i == -1) printf("\nError string");
                else countWord += i;
            } else addTranslation("NULL", label, trans);
            break;


    }

}

