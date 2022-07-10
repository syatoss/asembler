#include "../headers/DataTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/LabelTable.h"
#include "../headers/Translation.h"
#include "../headers/constants.h"

void addDataNodeToListTail(DataNode* node, DataList* list);
void addDataToListTail(Data* data, DataTable* table);

Data* newData(int lineCount, int startLine, Translation* translation,
              int hasLabel, char* labelName) {
    Data* data = (Data*)malloc(sizeof(Data));
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

void freeData(Data* data) {
    if (!data) return;
    if (data->labelName != NULL) free(data->labelName);
    free(data);
}

void printData(Data* data) {}

DataNode* newDataNode(Data* data) {
    DataNode* node = (DataNode*)malloc(sizeof(DataNode));
    node->next = NULL;
    node->data = data;
    return node;
}

DataList* newDataList() {
    DataList* list = (DataList*)malloc(sizeof(DataList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

DataTable* newDataTable() {
    DataTable* table = (DataTable*)malloc(sizeof(DataTable));
    table->rows = newDataList();
    table->entries = 0;
    return table;
}

void freeDataNode(DataNode* node) {
    if (!node) return;
    freeData(node->data);
    free(node);
}

void freeDataList(DataList* list) {
    if (!list) return;
    DataNode* current = list->head;
    while (current) {
        list->head = current->next;
        freeDataNode(current);
        current = list->head;
    }
    free(list->head);
}

void freeDataTable(DataTable* table) {
    if (!table) return;
    freeDataList(table->rows);
    free(table);
}

DataNode* getDataNodeByDc(DataList* list, int dc) {
    while (list->head) {
        if (list->head->data->startLine == dc) return list->head;
        list->head = list->head->next;
    }
    return NULL;
}

Data* getDataByDc(DataTable* table, int dc) {
    DataNode* node = getDataNodeByDc(table->rows, dc);
    return node ? node->data : NULL;
}

void addDataNodeToListTail(DataNode* node, DataList* list) {
    if (!node) return;
    if (!list->head) list->head = node;
    if (list->tail) list->tail->next = node;
    list->tail = node;
}

void addDataToListTail(Data* data, DataTable* table) {
    addDataNodeToListTail(newDataNode(data), table->rows);
}

void addDataToTable(Data* lable, DataTable* table) {
    addDataToListTail(lable, table);
    table->entries++;
}

void printDataNode(DataNode* node) {
    if (!node) return;
    printData(node->data);
    printf("\n");
}

void printDataList(DataList* list) {
    if (!list) return;
    while (list->head) {
        printDataNode(list->head);
        list->head = list->head->next;
    }
}

void printDataTable(DataTable* table) {
    printf("Table has %d entries:\n", table->entries);
    printDataList(table->rows);
}

