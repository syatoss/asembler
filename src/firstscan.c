#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>



#define N 80
#define MAX 250
#define NUMOFOPCODE 16
#define NUMOFREG 8
#define NUMOFDATACODE 5



//typedef struct LNode{
//    char* data[30];
//    struct LNode* next;
//}LNode;
//typedef struct List{
//    struct LNode *head, *tail;
//}List;



char *opcodeTable[NUMOFOPCODE]={"mov\0","cmp\0","add\0","sub\0","not\0","clr\0", "lea\0", "inc\0","dec\0",
                                "jmp\0", "bne\0", "get\0", "prn\0", "jsr\0", "rts\0", "hlt\0"};
char *regTable[NUMOFREG]={"r0\0","r1\0","r2\0","r3\0","r4\0","r5\0","r6\0","r7\0"};
char *dataCode[NUMOFDATACODE]={".data\0",".struct\0",".string\0",".entry\0", ".extern\0"};




void freeArr(char* );
char* srchWord(char*,int);
int isOpcode(char*, int n );
int isRegistr(char*, int n );
int isData(char*, int n );
//int isLabel(char*);
//void addToLable(char* arr);
//void insertNodeToTail(List* lst, LNode* newNode);
//void insertValueToTail(List *lst, char * newData);
//LNode* createNewNode(char* newData, LNode* next);
//int isEmpty(const List* lst);
//List makeEmptyList();


void firstscan () {
    FILE *file;
    char line[N]={};
    char word[N]={};
    char prevChar;
    int i=0, j=0;
    file = fopen("text.txt", "r");
    while ((line[i] = fgetc(file)) != EOF) {
        if(line[i] == ';' && i == 0){
            while ((line[i] = fgetc(file)) != '\n')
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
                printf("search the word: %s %s ", word, srchWord(word, j));
                j=0;
                freeArr(word); prevChar=' ';
            }
            line[i] = '\0';
            printf("\n%s\n", line);
            i = 0;
            freeArr(line);
        }
        else {

            if(isalpha(line[i]))
            {
                word[j]=line[i]; j++;
            }
            if(line[i] == ':')
            {
                if(isalnum(prevChar)){
                    word[j]='\0'; j=0;
                    printf("addToLabel:%s ", word);
                    freeArr(word); prevChar=' ';}
                else printf("Error ':' ");
                continue;
            }
            if(isspace(line[i]))
            {
                if(isalnum(prevChar) && word[0] != '\0') {
                    printf("search the word: %s %s ", word, srchWord(word, j));
                }
                freeArr(word); j=0;

            }
            if(isdigit(line[i]))
            {
                if(word[0] == '\0') printf("error");
                else { word[j]=line[i]; j++;}
            }
            if(line[i] == '.')
            {
                if(word[0] != '\0') { word[j]='\0'; j=0; printf("addToLabel:%s ", word);
                    freeArr(word);}
                if(word[0] == '\0') { word[j]=line[i]; j++;}
            }
            if(line[i] == ',')
            {
                if(!isspace(prevChar) || word[0] != '\0'){
                    printf("search the word: %s %s ", word, srchWord(word, j));
                    freeArr(word); j=0;}
            }


            prevChar=line[i];
            i++;

        }
    }
    line[i] = '\0';
    printf("\n%s\n", line);

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
char* srchWord(char* arr, int n )
{
    if(isOpcode(arr,n)!=-1) return "isOpcode";
    if(isRegistr(arr,n)!=-1) return "isRegistr";
    if(isData(arr,n)!=-1) return "isData";
    return "isLabel?";
}
int isOpcode(char* arr, int n )
{
    int i;
    for(i=0; i<NUMOFOPCODE; i++){
        if(strcmp(arr, opcodeTable[i])==0) return i;}
    return -1;
}
int isRegistr(char* arr, int n )
{
    int i;
    for(i=0; i<NUMOFREG; i++){
        if(strcmp(arr, regTable[i])==0) return i;}
    return -1;
}
int isData(char* arr, int n )
{
    int i;
    for(i=0; i<NUMOFDATACODE; i++){
        if(strcmp(arr, dataCode[i])==0) return i;}
    return -1;
}
//void addToLable(char* arr)
//{
//    int n=isLabel(arr);
//    if(n==-1){
//        labelTable[count]=arr;
//        count++;
//    }
//    else printf("Number of label:%d ", n);
//}

//int isLabel(char* arr)
//{
//    int i;
//    for(i=0; i<2; i++){
//        if(!strcmp(arr, labelTable[i])) return i;}
//    return -1;
//}
//LNode* createNewNode(char* newData, LNode* next)
//{
//    LNode * newNode=(LNode*) calloc(1,sizeof (LNode));
//    strcpy(newNode->data ,newData);
//    newNode->next = next;
//    return newNode;
//}
//void insertValueToTail(List *lst, char * newData)
//{
//    LNode * newNode = createNewNode(newData, NULL);
//    if(isEmpty(lst))
//        lst->head = lst->tail = newNode;
//    else{
//        lst->tail->next=newNode;
//        lst->tail=newNode;
//    }
//}
//void insertNodeToTail(List* lst, LNode* newNode)
//{
//    if(isEmpty(lst))
//        lst->head=lst->tail=newNode;
//    else
//    {
//        lst->tail->next=newNode;
//    }
//}
//List makeEmptyList()
//{
//    List lst;
//    lst.head = lst.tail =NULL;
//    return lst;
//}
//int isEmpty(const List* lst)
//{
//    return lst->head == NULL;
//}