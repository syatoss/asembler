#include "../headers/firstScan2.h"
#include <stdlib.h>

#include "../headers/asm_descriptor.h"
#include "../headers/constants.h"
#include "../headers/error_logger.h"
#include "../headers/line_paser.h"
#include "../headers/string_parsers.h"

const char WORD_END_DELIMITERS[WORD_END_DELIMITER_COUNT] = {'\n', '\0', ',',
                                                            ' ', '\t'};

extern AsmDescriptor *ds;

int isWordEndDelimiter(char c) {
  int i;
  for (i = 0; i < WORD_END_DELIMITER_COUNT; i++) {
    if (c == WORD_END_DELIMITERS[i])
      return true;
  }
  return false;
}

char *getNextWord(char *line, int *lastReadCharIndex) {
  char *word;
  int startIndex;
  while (is_white_space(line[*lastReadCharIndex]))
    (*lastReadCharIndex)++;
  startIndex = *lastReadCharIndex;
  while (!isWordEndDelimiter(line[(*lastReadCharIndex)++]))
    ;
  word = substring(line, startIndex, *lastReadCharIndex - 1);
  return word;
}

int isEntryExtern(char *word) { return true; }

void readEntryExtern(char *line) {}

int isLabelDef(char *word) { return true; }

void setLabelDefFlag(AsmDescriptor *ds, int isLabelDef) {}

int isDataDefinition(char *word) { return true; }

int getLabelDefFlag() { return true; }

void readRestOfDataLine(char *line, int *lastReadCharIndexa,
                        Label *currentLabel) {}

int isInstructionName(char *word) { return true; }

void readRestOfInstruction(char *line, int *lastReadCharIndex,
                           Label *currentLabel) {}

void firstScan(AsmDescriptor *ds) {
  char *word = NULL;
  char *labelName = NULL;
  Label *currentLabel = NULL;
  int lastReadCharIndex = 0;
  while (get_next_line(ds)) {
    if (is_comment_line(ds->line))
      continue;

    word = getNextWord(ds->line, &lastReadCharIndex);
    currentLabel = NULL;

    if (isEntryExtern(word)) {
      readEntryExtern(ds->line);
      freeMem(word, labelName);
      continue;
    }

    if (isLabelDef(word)) {
      setLabelDefFlag(ds, true);
      labelName = cp_string(getLabelNameFromDefinition(word));
      word = getNextWord(ds->line, &lastReadCharIndex);
    }

    if (isDataDefinition(word)) {
      if (getLabelDefFlag() == true)
        currentLabel->type = DATA;
      readRestOfDataLine(ds->line, &lastReadCharIndex, currentLabel);
      freeMem(word, labelName);
      continue;
    }

    if (isInstructionName(word)) {
      readRestOfInstruction(ds->line, &lastReadCharIndex, currentLabel);
      freeMem(word, labelName);
      continue;
    } else {
      log_error(ds->err_log, "invalid label def");
    }

    freeMem(word, labelName);
  }
}
