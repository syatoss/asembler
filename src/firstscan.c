
#include "../headers/firstscan.h"


#define N 80
#define MAXLABELNAME 30
#define NUMOFOPCODE 16
#define NUMOFREG 8
#define NUMOFDATACODE 5

#define addword() \
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
char *opcodeTwoOperand[5]={"mov\0", "cmp\0", "add\0", "sub\0", "lea\0"};
char *opcodeOneOperand[9]={"not\0", "clr\0", "inc\0", "dec\0","jmp\0","bne\0", "get\0", "prn\0", "jsr\0"};
enum WORD_TYPE wordType;

char label[MAXLABELNAME]={0};
char prevWord[N]={};
int dataCodeNumber=-1;
int opcodeNumber=-1;
char op1[MAXLABELNAME]={};
char op2[MAXLABELNAME]={};
int countWord=0;

int firstscan () {
    char line[N]={};
    while(get_next_line(ds))
    {
        strcpy(line,trim(ds->line));
        checkLine(line);
    }
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
            if(dataCodeNumber==-1 && opcodeNumber==-1 && !emptyArr(word)){
                addword();
            }
            prevChar = line[i];
            i++;
            continue;
        } else {
            if (line[i] == ':') {
                if (isalnum(prevChar)&&prevChar!=':'&& emptyArr(label)) {
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
                if (emptyArr(word) && dataCodeNumber==-1) printf("\nError number no data");
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
                    addword();
                }
                if (emptyArr(word)) {
                    word[j] = line[i];
                    j++;
                }
                if(prevChar=='.') printf("\nError '.' ");
                prevChar = line[i];
                i++;
                continue;
            }
            if (line[i] == ',') {
                if ((!isspace(prevChar) || !emptyArr(word))&&prevChar!=',') {
                    addword();
                    prevChar = line[i];
                }
                else printf("\nError ',' ");
                i++;
                continue;
            }
            if(!correctChar(line[i])) printf("\nError char");
            if(prevChar==':') printf("\nError string");
            else{
                word[j] = line[i];
                j++;
                prevChar = line[i];
            }
            i++;
        }
    }
    if (!emptyArr(word)) {
        addword();
    }
    if(prevChar==',') printf("\nError ','");
    freeArr(word);
    return;
}

int emptyArr(const char* arr) {
    if (arr[0] == '\0') return 1;
    return 0;
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

int checkNumberArr(char *arr) {
    int i = 0;
    if(arr[i]=='-' || arr[i]=='+') {
        if(arr[i+1]=='\0') {
            printf("Error number incorrect");
            return false;
        }
        i++;
    }
    while (arr[i]!='\0') {

        if (!isdigit(arr[i])) return false;
        i++;
    }
    return true;
}

int addString(char* str)
{
    int i=0;
    if(str[i]!='\"') return -1;
    while(str[i]!='\0')
    {
        i++;

        if(str[i]=='\"') {
            printf("\nadd char to DC code %s   ", aToBin('\0'));
            return i-2;
        }
        printf("\nadd char to DC code %s   ", aToBin(str[i]));
    }
    return i-2;
}
void addOperand(char* word)
{
    if (emptyArr(op1)) {
        strcpy(op1, word);

    } else {
        if (emptyArr(op2)) {
            strcpy(op2, word);
        } else printf(" \nOperand error");
    }
}
int correctChar(char ch)
{
    if (!((ch>='a'&& ch<='z')||(ch>='A'&&ch<='Z')||(ch>='0'&&ch<='9')||(ch=='\"')||(ch=='-')||(ch=='#')||(ch=='+'))) return false;
    return true;
}
void checkWord(char *word) {
    enum WORD_TYPE wordType;
    char *bin[WORD_SIZE];
    wordType = srchWord(word);
    switch (wordType) {

        case ISDATA:
            dataCodeNumber= isData(word);
            break;

        case ISNUMBER:
            if (word[0] == '.' || word[0] == '#') {
                if(isNumber(prevWord)) {
                    printf("\nError number: just natural numbers");
                    break;
                }
                if (word[0] == '#') {
                    addOperand(word);
                } else{
                    strcpy(*bin,intToBinary(atoi(word+1)));
                    shiftLeft(*bin,2);
                    printf("\nadd num  to IC code  %s", *bin);}
            } else {
                if(dataCodeNumber==0 || dataCodeNumber==1){
                    strcpy(*bin,intToBinary(atoi(word)));
                    if (dataCodeNumber==1) {
                        if (emptyArr(op1)) {
                            strcpy(op1, word);}
                        else printf("\nError struct");
                    }
                    printf("\nadd num  to DC code %s   ", *bin);}
                else printf("\nError code");}
            break;

        case ISOPCODE:
            if (opcodeNumber == -1) {
                opcodeNumber = isOpcode(word);
                printf("\nThe word is opcode %s %d ",word, opcodeNumber);
            } else printf("\nopcode error");
            break;

        case ISREGISTR:
            addOperand(word);
            if(!emptyArr(op1)&& emptyArr(op2)){
                printf("\nadd registr number to 1-4 bit in binary code");
            }
            if(!emptyArr(op1)&& !emptyArr(op2)){
                if(isRegistr(op1)!=-1) printf("\nadd registr number to 5-8 bit in prev binary code");
                else printf("\nadd registr number to 5-8 bit in new binary code" );
            }
            break;

        case UNKNOWN:
            addOperand(word);
            int i;
            if((dataCodeNumber==1 && !emptyArr(op2))||(dataCodeNumber==2 && !emptyArr(op1))){
                if(dataCodeNumber==1)  i=addString(op2);
                if(dataCodeNumber==2) i=addString(op1);
                if(i==-1) printf("\nError string");
                else countWord+=i;
            }
            else printf(" \nadd word to IC code  Null");
            break;
    }

}

//int isLabel(char* arr, LabelTable* table)
//{
//    return getLabelByName(ds->labelTable, arr);
//}
//void addDataLable(char* arr, int numOfData)
//{
//    switch (numOfData) {
//        case 0: addLabelToTable(newLabel(arr,ds, NONE, DATA), labelTable);
//            break;
//        case 1: addLabelToTable(newLabel(arr, ds, NONE, DATA), labelTable);
//            break;
//        case 2: addLabelToTable(newLabel(arr, ds, INTERNAL, DATA), labelTable);
//            break;
//        case 3: addLabelToTable(newLabel(arr, ds, INTERNAL, INSTRUCTION), labelTable);
//            break;
//        case 4: addLabelToTable(newLabel(arr, ds, INTERNAL, INSTRUCTION), labelTable);
//            break;
//        default: printf("Error addDataLable");
//
//    }
//}




