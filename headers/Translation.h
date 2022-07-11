#ifndef TRANSLATION_H
#define TRANSLATION_H

typedef struct {
    int lineCount;
    char** binary;
    char** base32;
} Translation;

char* binarayTo32(char* binary);
Translation* newTranslation(char** binary, int len);

#endif
