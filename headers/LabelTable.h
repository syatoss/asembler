#ifndef LABLE_TABLE_H
#define LABLE_TABLE_H

#include "./label.h"

typedef struct labelNode {
    struct labelNode* next;
    Label* label;
} LabelNode;

typedef struct {
    LabelNode* head;
    LabelNode* tail;
} LabelList;

typedef struct {
    int entries;
    LabelList* rows;
} LabelTable;

LabelNode* newLabelNode(Label* label);
LabelList* newLabelList();
LabelTable* newLabelTable();
void freeLabelNode(LabelNode* node);
void freeLabelList(LabelList* list);
void freeLabelTable(LabelTable* table);
int tableContainsLable(LabelTable* table, char* labelName);
Label* getLabelByName(LabelTable* table, char* labelName);
void addLabelToTable(Label* label, LabelTable* table);
void printLabelTable(LabelTable* table);

#endif

