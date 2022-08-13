#ifndef LABLE_TABLE_H
#define LABLE_TABLE_H

#include "./label.h"

typedef struct labelNode {
  struct labelNode *next;
  Label *label;
} LabelNode;

typedef struct {
  LabelNode *head;
  LabelNode *tail;
} LabelList;

typedef struct {
  int entries;
  int hasEntryLabels;
  int hasExternalLabels;
  LabelList *rows;
} LabelTable;

/* assines new memory for a new lable node and returns   the pointer */
LabelNode *newLabelNode(Label *label);
/* assines new memory for a new lable list and returns  the pointer */
LabelList *newLabelList();
/* assines new memory for a new lable table and returns the pointer */
LabelTable *newLabelTable();
/* frees the poiner to the lable node and all of its assosiated memmory
 * alocations */
void freeLabelNode(LabelNode *node);
/* frees the poiner to the lable list and all of its assosiated memmory
 * alocations */
void freeLabelList(LabelList *list);
/* frees the poiner to the lable table and all of its assosiated memmory
 * alocations */
void freeLabelTable(LabelTable *table);
/* check for label existance in the table */
int tableContainsLable(LabelTable *table, char *labelName);
/* returns the lable object from table given its name, returns null if the label
 * in not in the table */
Label *getLabelByName(LabelTable *table, char *labelName);
void addLabelToTable(Label *label, LabelTable *table);
/* fucntion for debuggin reasons */
void printLabelTable(LabelTable *table);

#endif
