
#include "../headers/Translation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/constants.h"
#include "../headers/string_parsers.h"

char base32[32][2] = {
    "!", "@", "#", "$", "%", "^", "&", "*", "<", ">", "a",
    "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l",
    "m", "n", "o", "p", "q", "r", "s", "t", "u", "v",

};

char *aToBin(char ascii) { return intToBinary((int)ascii); }

void flipBin(char *bin) {
  int first = 0;
  int last = strlen(bin) - 1;
  char pivot;
  if (!bin)
    return;
  while (last > first) {
    pivot = bin[last];
    bin[last] = bin[first];
    bin[first] = pivot;
    last--;
    first++;
  }
}

void printTranslation(Translation *trans) {
  int i;
  for (i = 0; i < trans->lineCount; i++) {
    printf("binary[%d]: %s\n", i,
           (trans->binary)[i] != NULL ? (trans->binary)[i] : "NULL");
    printf("base32[%d]: %s\n", i,
           (trans->base32)[i] != NULL ? (trans->base32)[i] : "NULL");
  }
}

void setARE(char *bin, enum ARE are) {
  if (bin == NULL)
    return;

  if (are == A) {
    bin[WORD_SIZE - 1] = '0';
    bin[WORD_SIZE - 2] = '0';
  }
  if (are == R) {
    bin[WORD_SIZE - 1] = '0';
    bin[WORD_SIZE - 2] = '1';
  }

  if (are == E) {
    bin[WORD_SIZE - 1] = '1';
    bin[WORD_SIZE - 2] = '0';
  }
}

void shiftLeft(char *bin, int shiftSize) {
  flipBin(bin);
  shiftRight(bin, shiftSize);
  flipBin(bin);
}

void shiftRight(char *bin, int shiftSize) {
  int i;
  char *pivot = cp_string(bin);
  if (shiftSize > WORD_SIZE)
    shiftSize = WORD_SIZE;
  if (shiftSize < 0)
    return;
  for (i = 0; i + shiftSize < WORD_SIZE; i++) {
    bin[i + shiftSize] = pivot[i];
  }
  for (i = 0; i < shiftSize; i++) {
    bin[i] = '0';
  }
  free(pivot);
  return;
}

unsigned long int binToDec(char *binary) {
  int currentBitIndex;
  char currentBit;
  unsigned long int one = 1;
  unsigned long int dec = 0;
  currentBitIndex = strlen(binary) - 1;
  currentBit = binary[currentBitIndex];
  while (currentBitIndex >= 0) {
    if (currentBit == '1')
      dec = dec | one;
    one <<= 1;
    currentBitIndex--;
    currentBit = binary[currentBitIndex];
  }
  return dec;
}

char *uIntToBinary(unsigned int uInt) {
  char *binUInt = (char *)malloc(sizeof(char) * WORD_SIZE + 1);
  unsigned long int bit = 1;
  int binUIntCurrentBit = WORD_SIZE - 1;
  binUInt[WORD_SIZE] = '\0';
  while (binUIntCurrentBit >= 0) {
    binUInt[binUIntCurrentBit] = bit & uInt ? '1' : '0';
    bit <<= 1;
    binUIntCurrentBit--;
  }
  return binUInt;
}

void completionTo2(char *binVal) {
  int bit = WORD_SIZE - 1;
  /* avoids changes until the first on bit */
  while (bit >= 0) {
    if (binVal[bit] == '0') {
      bit--;
    } else {
      bit--;
      break;
    }
  }
  /* flips all bits after the first turned */
  while (bit >= 0) {
    binVal[bit] = binVal[bit] == '0' ? '1' : '0';
    bit--;
  }
}

char *intToBinary(int num) {
  char *binInt;
  unsigned int absVal;
  int negative = num < 0;
  absVal = negative ? -num : num;
  binInt = uIntToBinary(absVal);
  if (negative)
    completionTo2(binInt);
  return binInt;
}

char *binaryToBase32(char *binary) {
  char *base32String = NULL;
  char *b32;
  char *msb;
  char *lsb;
  char *ms32b;
  char *ls32b;
  msb = substring(binary, 0, 5);
  lsb = substring(binary, 5, 5);
  ms32b = base32[binToDec(msb)];
  ls32b = base32[binToDec(lsb)];
  b32 = cat_strings(base32String, ms32b, ls32b, NULL);
  free(msb);
  free(lsb);
  return b32;
}

char *intToBase32(int num) {
  char *binInt;
  char *base32Int;
  binInt = intToBinary(num);
  base32Int = binaryToBase32(binInt);
  free(binInt);
  return base32Int;
}

void addTranslation(char *bin, char *label, Translation *trans) {
  trans->binary = (char **)realloc(trans->binary,
                                   (sizeof(char *) * (trans->lineCount + 1)));
  trans->base32 = (char **)realloc(trans->base32,
                                   (sizeof(char *) * (trans->lineCount + 1)));
  if (bin == NULL) {
    (trans->base32)[trans->lineCount] = NULL;
    (trans->binary)[trans->lineCount] = NULL;
    if (trans->lineCount < MAX_WORDS_PER_INSTRUCTION)
      trans->nulls[trans->lineCount] = cp_string(label);
  } else {
    (trans->binary)[trans->lineCount] = cp_string(bin);
    (trans->base32)[trans->lineCount] = binaryToBase32(bin);
    if (trans->lineCount < MAX_WORDS_PER_INSTRUCTION)
      trans->nulls[trans->lineCount] = NULL;
  }
  trans->lineCount++;
}

void updateTranslationAtIndex(Translation *trans, int index, char *bin,
                              char *label) {
  if (index >= trans->lineCount)
    return;
  if (trans->binary[index] != NULL)
    free(trans->binary[index]);
  if (trans->base32[index] != NULL)
    free(trans->base32[index]);
  trans->binary[index] = NULL;
  trans->base32[index] = NULL;
  if (label == NULL)
    trans->nulls[index] = NULL;
  else
    trans->nulls[index] = cp_string(label);
  if (bin == NULL)
    return;
  trans->binary[index] = cp_string(bin);
  trans->base32[index] = binaryToBase32(bin);
}

Translation *newTranslation() {
  int i;
  Translation *trans = (Translation *)malloc(sizeof(Translation));
  trans->binary = NULL;
  trans->base32 = NULL;
  trans->lineCount = 0;
  for (i = 0; i < MAX_WORDS_PER_INSTRUCTION; i++) {
    trans->nulls[i] = NULL;
  }
  return trans;
}

void freeTranslation(Translation *trans) {
  int i;
  if (!trans)
    return;
  for (i = 0; i < trans->lineCount; i++) {
    if (trans->base32[i] != NULL)
      free((trans->base32)[i]);
    if (trans->binary[i] != NULL)
      free((trans->binary)[i]);
  }
  for (i = 0; i < MAX_WORDS_PER_INSTRUCTION; i++) {
    if (trans->nulls[i] != NULL)
      free(trans->nulls[i]);
  }
  free(trans->base32);
  free(trans->binary);
  free(trans);
}
