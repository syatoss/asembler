#include "../headers/LabelTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/constants.h"

void addLabelNodeToListTail(LabelNode* node, LabelList* list);
void addLabelToListTail(Label* label, LabelTable* table);

LabelNode* newLabelNode(Label* label) {
    LabelNode* node = (LabelNode*)malloc(sizeof(LabelNode));
    node->next = NULL;
    node->label = label;
    return node;
}

LabelList* newLabelList() {
    LabelList* list = (LabelList*)malloc(sizeof(LabelList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

LabelTable* newLabelTable() {
    LabelTable* table = (LabelTable*)malloc(sizeof(LabelTable));
    table->rows = newLabelList();
    table->entries = 0;
    table->hasEntryLabels = false;
    table->hasExternalLabels = false;
    return table;
}

void freeLabelNode(LabelNode* node) {
    if (!node) return;
    freeLabel(node->label);
    free(node);
}

void freeLabelList(LabelList* list) {
    LabelNode* current;
    if (!list) return;
    current = list->head;
    while (current) {
        list->head = current->next;
        freeLabelNode(current);
        current = list->head;
    }
    free(list->head);
}

void freeLabelTable(LabelTable* table) {
    if (!table) return;
    freeLabelList(table->rows);
    free(table);
}

LabelNode* getLabelNodeByName(LabelList* list, char* name) {
    while (list->head) {
        if (strcmp(list->head->label->name, name) == 0) return list->head;
        list->head = list->head->next;
    }
    return NULL;
}

Label* getLabelByName(LabelTable* table, char* name) {
    LabelNode* node = getLabelNodeByName(table->rows, name);
    return node ? node->label : NULL;
}

int tableContainsLable(LabelTable* table, char* labelName) {
    return getLabelByName(table, labelName) ? true : false;
}

void addLabelNodeToListTail(LabelNode* node, LabelList* list) {
    if (!node) return;
    if (!list->head) list->head = node;
    if (list->tail) list->tail->next = node;
    list->tail = node;
}

void addLabelToListTail(Label* label, LabelTable* table) {
    addLabelNodeToListTail(newLabelNode(label), table->rows);
}

void addLabelToTable(Label* lable, LabelTable* table) {
    addLabelToListTail(lable, table);
    table->entries++;
    table->hasEntryLabels = table->hasEntryLabels || lable->status == INTERNAL;
    table->hasExternalLabels =
        table->hasExternalLabels || lable->status == EXTERNAL;
}

void printLabelNode(LabelNode* node) {
    if (!node) return;
    printLabel(node->label);
    printf("\n");
}

void printLabelList(LabelList* list) {
    if (!list) return;
    while (list->head) {
        printLabelNode(list->head);
        list->head = list->head->next;
    }
}

void printLabelTable(LabelTable* table) {
    printf("Table has %d entries:\n", table->entries);
    printLabelList(table->rows);
}

