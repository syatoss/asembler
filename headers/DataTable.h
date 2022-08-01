#ifndef DATA_TABLE_H
#define DATA_TABLE_H

#include "./Translation.h"

typedef struct {
  int lineCount; /*how many lines the translation takes*/
  int startLine; /*dc*/
  int lineNumInAsmFile;
  Translation *translation;
  int hasLabel;    /*true/false*/
  char *labelName; /*string/NULL*/
} AsmRow;

typedef struct asmRowNode {
  struct asmRowNode *next;
  AsmRow *data;
} AsmRowNode;

typedef struct {
  AsmRowNode *head;
  AsmRowNode *tail;
} AsmRowList;

typedef struct {
  int entries; /* row count */
  AsmRowList *rows;
  int translationCounter; /*ic or dc depending on context*/
} AsmTranslationTable;

/* creats a new row in the as translation table */
AsmRow *newAsmRow(int lineCount, int startLine, int lineNumInAsmFile,
                  Translation *translation, int hasLabel, char *labelName);
/* frees all alocated space for the row in a asm translation table */
void freeAsmRow(AsmRow *data);
void printAsmRow(AsmRow *data);
AsmRowNode *newAsmRowNode(AsmRow *data);
AsmRowList *newAsmRowList();
/* returns a new instance of a translation table */
AsmTranslationTable *newAsmTranslationTable();
void freeAsmRowNode(AsmRowNode *node);
void freeAsmRowList(AsmRowList *list);
/* frees the table and all of its content */
void freeAsmTranslationTable(AsmTranslationTable *table);
/* recieves the start line (dc or ic) and return the node */
AsmRowNode *getAsmRowNodeByStartLine(AsmRowList *list, int startLine);
/* recieves the start line (dc or ic) and return the table row content */
AsmRow *getAsmRowByStartLine(AsmTranslationTable *table, int startLine);
void addAsmRowNodeToListTail(AsmRowNode *node, AsmRowList *list);
void addAsmRowToListTail(AsmRow *data, AsmTranslationTable *table);
void addAsmRowToTable(AsmRow *lable, AsmTranslationTable *table);
void printAsmRowNode(AsmRowNode *node);
void printAsmRowList(AsmRowList *list);
/* print the asm table for debuggin reasons */
void printAsmTranslationTable(AsmTranslationTable *table);
/* an interface for loopin the table rows */
void forEachRow(AsmTranslationTable *tb, void callback(AsmRow *));

#endif
