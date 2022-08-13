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

/* allocates new memmory for label and assignes default values */
Label *newLabel(char *name, int lineOfApearance, enum LABEL_STATUS status,
                enum LABEL_TYPE type);
/* frees the label pointer and all assosiated memory*/
void freeLabel(Label *label);
/* function for debugging */
void printLabel(Label *label);
/* racieves a name of a label from its definition line and returns the name
 * without trailing charcters */
char *getLabelNameFromDefinition(char *labelDefinition);

#endif
