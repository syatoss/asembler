#include "../headers/DataTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/LabelTable.h"
#include "../headers/Translation.h"
#include "../headers/constants.h"

void addAsmRowNodeToListTail(AsmRowNode* node, AsmRowList* list);
void addAsmRowToListTail(AsmRow* data, AsmTranslationTable* table);

AsmRow* newAsmRow(int lineCount, int startLine, Translation* translation,
                  int hasLabel, char* labelName) {
    AsmRow* data = (AsmRow*)malloc(sizeof(AsmRow));
    data->lineCount = lineCount;
    data->startLine = startLine;
    data->translation = translation;
    data->hasLabel = hasLabel;
    data->labelName = NULL;
    if (data->hasLabel) {
        data->labelName = (char*)malloc(sizeof(char) * strlen(labelName) + 1);
        strcpy(data->labelName, labelName);
    }
    return data;
}

void freeAsmRow(AsmRow* data) {
    if (!data) return;
    freeTranslation(data->translation);
    if (data->labelName != NULL) free(data->labelName);
    free(data);
}

void printAsmRow(AsmRow* data) {}

AsmRowNode* newAsmRowNode(AsmRow* data) {
    AsmRowNode* node = (AsmRowNode*)malloc(sizeof(AsmRowNode));
    node->next = NULL;
    node->data = data;
    return node;
}

AsmRowList* newAsmRowList() {
    AsmRowList* list = (AsmRowList*)malloc(sizeof(AsmRowList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

AsmTranslationTable* newAsmTranslationTable() {
    AsmTranslationTable* table =
        (AsmTranslationTable*)malloc(sizeof(AsmTranslationTable));
    table->rows = newAsmRowList();
    table->entries = 0;
    return table;
}

void freeAsmRowNode(AsmRowNode* node) {
    if (!node) return;
    freeAsmRow(node->data);
    free(node);
}

void freeAsmRowList(AsmRowList* list) {
    AsmRowNode* current;
    if (!list) return;
    current = list->head;
    while (current) {
        list->head = current->next;
        freeAsmRowNode(current);
        current = list->head;
    }
    free(list->head);
}

void freeAsmTranslationTable(AsmTranslationTable* table) {
    if (!table) return;
    freeAsmRowList(table->rows);
    free(table);
}

AsmRowNode* getAsmRowNodeByStartLine(AsmRowList* list, int startLine) {
    while (list->head) {
        if (list->head->data->startLine == startLine) return list->head;
        list->head = list->head->next;
    }
    return NULL;
}

AsmRow* getAsmRowByStartLine(AsmTranslationTable* table, int startLine) {
    AsmRowNode* node = getAsmRowNodeByStartLine(table->rows, startLine);
    return node ? node->data : NULL;
}

void addAsmRowNodeToListTail(AsmRowNode* node, AsmRowList* list) {
    if (!node) return;
    if (!list->head) list->head = node;
    if (list->tail) list->tail->next = node;
    list->tail = node;
}

void addAsmRowToListTail(AsmRow* data, AsmTranslationTable* table) {
    addAsmRowNodeToListTail(newAsmRowNode(data), table->rows);
}

void addAsmRowToTable(AsmRow* lable, AsmTranslationTable* table) {
    addAsmRowToListTail(lable, table);
    table->entries++;
}

void printAsmRowNode(AsmRowNode* node) {
    if (!node) return;
    printAsmRow(node->data);
    printf("\n");
}

void printAsmRowList(AsmRowList* list) {
    if (!list) return;
    while (list->head) {
        printAsmRowNode(list->head);
        list->head = list->head->next;
    }
}

void printAsmTranslationTable(AsmTranslationTable* table) {
    printf("Table has %d entries:\n", table->entries);
    printAsmRowList(table->rows);
}

