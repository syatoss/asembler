#ifndef LABLE_H
#define LABLE_H

#define MAX_LABEL_LENGHT 30

enum LABEL_STATUS { NONE, INTERNAL, EXTERNAL };
enum LABEL_TYPE { INSTRUCTION, DATA };

typedef struct {
  char *name;
  int lineOfApearance;
  enum LABEL_STATUS status; /* internal/external/none */
  enum LABEL_TYPE type;     /* instruction or data */
} Label;

Label *newLabel(char *name, int lineOfApearance, enum LABEL_STATUS status,
                enum LABEL_TYPE type);
void freeLabel(Label *label);
void printLabel(Label *label);
char *getLabelNameFromDefinition(char *labelDefinition);

#endif
