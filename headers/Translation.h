#ifndef TRANSLATION_H
#define TRANSLATION_H

enum ARE { A, R, E };

typedef struct {
    int lineCount;
    char** binary;
    char** base32;
} Translation;

char* binaryToBase32(char* binary);
Translation* newTranslation();
void freeTranslation(Translation* trans);
unsigned long int binToDec(char* binary);
char* uIntToBinary(unsigned int uInt);
void completionTo2(char* binVal);
char* intToBinary(int num);
void flipBin(char* bin);
void shiftRight(char* bin, int shiftSize);
void shiftLeft(char* bin, int shiftSize);
char* aToBin(char ascii);
void addTranslation(char* bin, Translation* trans);
void printTranslation(Translation* trans);
void updateTranslationAtIndex(Translation* trans, int index, char* bin);
void setARE(char* bin, enum ARE are);

#endif
