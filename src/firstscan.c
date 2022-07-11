#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "label.h"
#include "LabelTabel.h"



#define N 80
#define MAXLABELNAME 30
#define NUMOFOPCODE 16
#define NUMOFREG 8
#define NUMOFDATACODE 5




char *opcodeTable[NUMOFOPCODE]={"mov\0","cmp\0","add\0","sub\0","not\0","clr\0", "lea\0", "inc\0","dec\0",
                                "jmp\0", "bne\0", "get\0", "prn\0", "jsr\0", "rts\0", "hlt\0"};
char *regTable[NUMOFREG]={"r0\0","r1\0","r2\0","r3\0","r4\0","r5\0","r6\0","r7\0"};
char *dataCode[NUMOFDATACODE]={".data\0",".struct\0",".string\0",".entry\0", ".extern\0"};




void freeArr(char*);
char* srchWord(char*);
int isOpcode(char*);
int isNumber(char*);
int isRegistr(char*);
int isData(char*);
//int isLabel(char*);
void addToLable(char* );



int firstscan () {
    FILE *file;
    char line[N]={};
    char word[N]={};
    int symbolSetting=0;
    char label[MAXLABELNAME]={};
    int countWord=0;
    char prevChar;
    int i=0, j=0;
    file = fopen("text.txt", "r");
    while ((line[i] = fgetc(file)) != EOF) {
        if(line[i] == ';' && i == 0){
            while ((line[i] = fgetc(file)) != '\n' && line[i]!= EOF)
            {
                i++;
            }
            i=0;
            continue;
        }
        if (line[i] == '\n') {
            if(isalnum(prevChar) || ispunct(prevChar))
            {
                word[j]='\0';
                printf("search the word: %s %s ", word, srchWord(word));
                j=0;
                freeArr(word); prevChar=' ';
            }
            printf("\n");
            i = 0;
            freeArr(line);
            continue;

        }
        if(isspace(line[i]))
        {
            if(isalnum(prevChar) && word[0] != '\0')
            {
                printf("search the word: %s %s ", word, srchWord(word));
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
                    symbolSetting=1;
                    strcpy(label, word);
                    printf("addToLabel:%s ", word);
                    freeArr(word); prevChar=' ';}
                else printf("Error ':' ");
                continue;
            }
            if(isdigit(line[i]))
            {
                if(word[0] == '\0') printf("error");
                else { word[j]=line[i]; j++;}
                continue;
            }
            if(line[i] == '.')
            {
                if(word[0] != '\0') { word[j]='\0'; j=0; printf("searchLabel:%s ", word);
                    freeArr(word);}
                if(word[0] == '\0') { word[j]=line[i]; j++;}
                continue;
            }
            if(line[i] == ',')
            {
                if(!isspace(prevChar) || word[0] != '\0'){
                    printf("search the word: %s %s ", word, srchWord(word));
                    freeArr(word); j=0;}
                continue;
            }

            word[j]=line[i];
            j++;
            prevChar=line[i];
            i++;

        }
    }
    freeArr(line);
    freeArr(word);
    fclose(file);
}

void freeArr(char* line)
{
    int i;
    for(i=0; i<N; i++)
    {
        line[i]='\0';
    }
}
char* srchWord(char* arr)
{
    if(isOpcode(arr)!=-1) return "isOpcode";
    if(isRegistr(arr)!=-1) return "isRegistr";
    if(isData(arr)!=-1) return "isData";
    if(isNumber(arr)) return "isNumber";
    return "isLabel?";
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
        if(strcmp(arr, dataCode[i])==0) {printf("its data number %d ", i);return i;}}
    return -1;
}
int isNumber(char* arr)
{
    char buf[N]={};
    strcpy(buf,arr);
    if(arr[0]=='.'|| arr[0]=='#'){
        strcpy(buf,arr+1);
    }
    return atoi(buf);
}

//void addDataLable(char* arr, int numOfData)
//{
//    switch (numOfData) {
//        case 0: addLabelToTable(newLabel(arr, ++line, status1, DATA), labelTable);
//            break;
//        case 1: addLabelToTable(newLabel(arr, ++line, status1, DATA), labelTable);
//            break;
//        case 2: addLabelToTable(newLabel(arr, ++line, status1, DATA), labelTable);
//            break;
//        case 3: addLabelToTable(newLabel(arr, ++line, status1, INSTRUCTION), labelTable);
//            break;
//        case 4: addLabelToTable(newLabel(arr, ++line, status1, INSTRUCTION), labelTable);
//            break;
//        default: printf("Error addDataLable");
//
//    }
//}

//int isLabel(char* arr, LabelTable* table)
//{
//    return getLabelByName(table, arr);
//}


