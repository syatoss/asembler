#ifndef DATA_TABLE_H
#define DATA_TABLE_H

#include "./Translation.h"

typedef struct {
    int lineCount;
    int startLine; /*dc*/
    Translation* translation;
    int hasLabel;    /*true/false*/
    char* labelName; /*string/NULL*/
} Data;

typedef struct dataNode {
    struct dataNode* next;
    Data* data;
} DataNode;

typedef struct {
    DataNode* head;
    DataNode* tail;
} DataList;

typedef struct {
    int entries;
    DataList* rows;
} DataTable;

#endif
