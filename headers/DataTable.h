#ifndef DATA_TABLE_H
#define DATA_TABLE_H

#include "./Translation.h"

typedef struct {
    int lineCount;
    int startLine; /*dc*/
    Translation* translation;
    int hasLabel;    /*true/false*/
    char* labelName; /*string/NULL*/
} AsmRow;

typedef struct asmRowNode {
    struct asmRowNode* next;
    AsmRow* data;
} AsmRowNode;

typedef struct {
    AsmRowNode* head;
    AsmRowNode* tail;
} AsmRowList;

typedef struct {
    int entries;
    AsmRowList* rows;
} AsmTranslationTable;

#endif
