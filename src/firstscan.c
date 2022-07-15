
#include "../headers/firstscan.h"


#define N 80
#define MAXLABELNAME 30
#define NUMOFOPCODE 16
#define NUMOFREG 8
#define NUMOFDATACODE 5

enum WORD_TYPE { ISDATA, ISOPCODE, ISREGISTR, ISNUMBER, UNKNOWN };


char *opcodeTable[NUMOFOPCODE]={"mov\0","cmp\0","add\0","sub\0","not\0","clr\0", "lea\0", "inc\0","dec\0",
                                "jmp\0", "bne\0", "get\0", "prn\0", "jsr\0", "rts\0", "hlt\0"};
char *regTable[NUMOFREG]={"r0\0","r1\0","r2\0","r3\0","r4\0","r5\0","r6\0","r7\0"};
char *dataCode[NUMOFDATACODE]={".data\0",".struct\0",".string\0",".entry\0", ".extern\0"};
enum WORD_TYPE wordType;

char label[MAXLABELNAME]={0};
int dataCodeNumber=-1;
int opcodeNumber=-1;
int op1=-1;
int op2=-1;
int countWord=0;

int firstscan () {
    char line[N]={};
    while(get_next_line(ds))
    {
        strcpy(line,trim(ds->line));
        checkLine(line);
    }
}
void checkLine(char* line)
{
    char word[N]={};
    char prevChar;
    int i=0, j=0; // r2, LENGTH\0
    while (line[i] != '\0') {
        if(line[i] == ';' && i == 0){
            return;
        }
        if(isspace(line[i]))
        {
            if(isalnum(prevChar) && word[0] != '\0')
            {
                countWord++;
                checkWord(word);
                freeArr(word); j=0;
            }
            prevChar=line[i];
            i++;
            continue;
        }
        else {
            if(line[i] == ':')
            {
                if(isalnum(prevChar)){
                    word[j]='\0'; j=0;
                    strcpy(label, word);
                    freeArr(word); prevChar=' ';}
                else printf("Error ':' ");
                i++;
                continue;
            }
            if(isdigit(line[i]))
            {
                if(word[0] == '\0') printf("error");
                else { word[j]=line[i]; j++;}
                i++;
                continue;
            }
            if(line[i] == '.')
            {
                if(word[0] != '\0') { word[j]='\0'; j=0;
                    countWord++;
                    checkWord(word);
                    freeArr(word);}
                if(word[0] == '\0') { word[j]=line[i]; j++;}
                continue;
            }
            if(line[i] == ',')
            {
                if(!isspace(prevChar) || word[0] != '\0'){
                    countWord++;
                    checkWord(word);
                    freeArr(word); j=0;}
                continue;
            }
            word[j]=line[i];
            j++;
            prevChar=line[i];

            i++;
            if (line[i] == '\0') {
                if(word[0]!='\0')
                {
                    word[j]='\0';
                    countWord++;
                    checkWord(word);
                    freeArr(word);
                }
                return;
            }
        }
    }
    freeArr(word);
}
void freeArr(char* line)
{
    int i;
    for(i=0; i<N; i++)
    {
        line[i]='\0';
    }
}
enum WORD_TYPE srchWord(char* arr)
{
    if(isOpcode(arr)!=-1) return ISOPCODE;
    if(isRegistr(arr)!=-1) return ISREGISTR;
    if(isData(arr)!=-1) return ISDATA;
    if(isNumber(arr)) return ISNUMBER;
    return UNKNOWN;
}
int isOpcode(char* arr )
{
    int i;
    for(i=0; i<NUMOFOPCODE; i++){
        if(strcmp(arr, opcodeTable[i])==0) return i;}
    return -1;
}
int isRegistr(char* arr )
{
    int i;
    for(i=0; i<NUMOFREG; i++){
        if(strcmp(arr, regTable[i])==0) return i;}
    return -1;
}
int isData(char* arr )
{
    int i;
    for(i=0; i<NUMOFDATACODE; i++){
        if(strcmp(arr, dataCode[i])==0) return i;
    }
    return -1;
}
int isNumber(char* arr)
{
    char buf[30]={};
    strcpy(buf,arr);
    if(arr[0]=='.'|| arr[0]=='#'){
        strcpy(buf,arr+1);
    }
    if(checkNumberArr(buf)) return true;
    return false;
}
int checkNumberArr(char* arr)
{
    int i=0;
    if(arr[i]=='-' || arr[i]=='+') {
        if(arr[i+1]=='\0') {
            printf("Error number incorrect");
            return false;
        }
    }
    while(arr[i]!='\0')
    {
        if(isdigit(arr[i]) )  i++;
        else return false;
    }
    return true;
}


void checkWord(const char* word){
    enum WORD_TYPE wordType;
    wordType=srchWord(word);
    switch (wordType) {
        case ISDATA:
            break;
       case ISNUMBER: if(word[0]=='.') //add number atoi(word) to binary machine code with <<
                      if(word[0]=='#') //add number atoi(word) to binary machine code with 2's complement
            break;
        case ISOPCODE:
            if(opcodeNumber==-1) opcodeNumber= isOpcode(word);
            //add binary machine code of opcode
            printf("opcode error");
            break;
        case ISREGISTR: break;
        case UNKNOWN: if(op1==-1) op1=1; //add operand to opcode
                      else {
                             if (op2 == -1) op2 = 1; //add operand to opcode
                             else printf("Operand error");
                      }
                      //add binary machine code Null
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




