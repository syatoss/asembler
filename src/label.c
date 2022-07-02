#include "../headers/label.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Label* newLabel(char* name, int lineOfApearance, enum LABEL_STATUS status,
                enum LABEL_TYPE type) {
    Label* label = (Label*)malloc(sizeof(Label));
    label->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(label->name, name);
    label->lineOfApearance = lineOfApearance;
    label->status = status;
    label->type = type;
    return label;
}

void freeLabel(Label* label) {
    if (!label) return;
    free(label->name);
    free(label);
}

void printLabel(Label* label) {
    const char* labelStatuses[3] = {"None", "Internal", "External"};
    const char* labelTypes[2] = {"Instruction", "Data"};
    if (!label) return;
    printf("Label Name: %s\nApeard in line: %d\nStatus: %s\nType: %s\n",
           label->name, label->lineOfApearance, labelStatuses[label->status],
           labelTypes[label->type]);
}
