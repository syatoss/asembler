
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

char* uIntToBinary(unsigned int uInt) {
    char* binUInt = (char*)malloc(sizeof(char) * WORD_SIZE + 1);
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


void completionTo2(char* binVal) {
    int bit = WORD_SIZE - 1;
    while (bit >= 0) {
        if (binVal[bit] == '0') {
            bit--;
        } else {
            bit--;
            break;
        }
        while (bit >= 0) {
            binVal[bit] = binVal[bit] == '0' ? '1' : '0';
            bit--;
        }
    }
}

char* intToBinary(int num) {
    char* binInt;
    unsigned int absVal;
    int negative = num < 0;
    absVal = negative ? -num : num;
    binInt = uIntToBinary(absVal);
    if (negative) completionTo2(binInt);
    return binInt;
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

