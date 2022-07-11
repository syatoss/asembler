
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

unsigned long int binToDec(char* binary) {
    int currentBitIndex;
    char currentBit;
    unsigned long int one = 1;
    unsigned long int dec = 0;
    currentBitIndex = strlen(binary) - 1;
    currentBit = binary[currentBitIndex];
    while (currentBitIndex >= 0) {
        if (currentBit == '1') dec = dec | one;
        one <<= 1;
        currentBitIndex--;
        currentBit = binary[currentBitIndex];
    }
    return dec;
}

char* binaryToBase32(char* binary) {
    char* base32String = NULL;
    char msb[6];
    char lsb[6];
    char* ms32b;
    char* ls32b;
    substring(msb, binary, 0, 5);
    substring(lsb, binary, 5, 5);
    ms32b = base32[binToDec(msb)];
    ls32b = base32[binToDec(lsb)];
    return cat_strings(base32String, ms32b, ls32b, NULL);
}

Translation* newTranslation(char** binary, int len) {
    int i;
    Translation* trans = (Translation*)malloc(sizeof(Translation));
    trans->binary = (char**)malloc(sizeof(char*));
    trans->base32 = (char**)malloc(sizeof(char*));
    trans->lineCount = len;
    for (i = 0; i < len; i++) {
        if ((trans->binary)[i] == NULL) continue;
        (trans->binary)[i] = cp_string(binary[i]);
        (trans->base32)[i] = binaryToBase32(binary[i]);
    }
    return trans;
}

void freeTranslation(Translation* trans) {
    int i;
    if (!trans) return;
    for (i = 0; i < trans->lineCount; i++) {
        free((trans->base32)[i]);
        free((trans->binary)[i]);
    }
    free(trans->base32);
    free(trans->binary);
    free(trans);
}

