#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/LabelTable.h"
#include "../headers/asm_descriptor.h"
#include "../headers/constants.h"
#include "../headers/line_paser.h"
#include "../headers/string_parsers.h"

extern AsmDescriptor* ds;

int isEntry(char* word) { return strcpy(word, ENTRY_LABEL) == 0; }

int shouldSkipLine(char* line) {
    int shouldSkip = false;
    StrArr* words = get_line_words(line);
    shouldSkip = shouldSkip || is_comment_line(line);
    shouldSkip = words->length < 2;
    shouldSkip = shouldSkip || !isEntry(words->strings[0]);
    free_str_arr(words);
    return shouldSkip;
}

char* getEntryLabelNameFromLine(char* line) {
    StrArr* lineWords = get_line_words(line);
    char* labelName = cp_string(lineWords->strings[1]);
    free_str_arr(lineWords);
    return labelName;
}

void setInternalLabel(char* line) {
    char* labelName;
    labelName = getEntryLabelNameFromLine(line);
    Label* label = getLabelByName(ds->lable_tb, labelName);
    label->status = INTERNAL;
}

void handleAsmLine() {
    char* line = trim(ds->line);
    if (shouldSkipLine(line)) return;
    setInternalLabel(line);
}

void completeInstructionTranslation(AsmDescriptor* ds) {}

void secondScan() {
    int i = 0;
    while (get_next_line(ds)) {
        handleAsmLine();
    }
    completeInstructionTranslation(ds);
}
