
#include "../headers/Translation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/string_parsers.h"

char* getBase32Translation(char* binary) { return cp_string(binary); }

Translation* newTranslation(char** binary, int len) {
    int i;
    Translation* trans = (Translation*)malloc(sizeof(Translation));
    trans->binary = (char**)malloc(sizeof(char*));
    trans->base32 = (char**)malloc(sizeof(char*));
    trans->lineCount = len;
    for (i = 0; i < len; i++) {
        (trans->binary)[i] = cp_string(binary[i]);
        (trans->base32)[i] = getBase32Translation(binary[i]);
    }
    return trans;
}

