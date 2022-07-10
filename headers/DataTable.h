#ifndef DATA_TABLE_H
#define DATA_TABLE_H

#include "./Translation.h"

typedef struct {
    int lineCount;
    int startLine;
    Translation* translation;
    int hasLabel;
    char* labelName;
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
