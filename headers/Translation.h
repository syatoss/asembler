#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "./constants.h"

enum ARE { A, R, E };

typedef struct {
  int lineCount;
  char *nulls[MAX_WORDS_PER_INSTRUCTION];
  char **binary;
  char **base32;
} Translation;

char *binaryToBase32(char *binary);
/* allocates new memmory for translation, assignes default values and returns a
 * pointer to the newly allocated space */
Translation *newTranslation();
/* frees the pointer and all assosiated allocated memory */
void freeTranslation(Translation *trans);
/* returns the decimal value of a given binary representation */
unsigned long int binToDec(char *binary);
/* returns the binary value of a given unsigned int representation */
char *uIntToBinary(unsigned int uInt);
/* assingens the 2's compliment of binVal to binVal */
void completionTo2(char *binVal);
/* converts an integer to its binary form, returns a pointer to newly allocated
 * string representing the binary form */
char *intToBinary(int num);
/* flips the provided string in place */
void flipBin(char *bin);
/* analogus to shift right orerand in c >> for string representation of binary
 * number, performed in place */
void shiftRight(char *bin, int shiftSize);
/* analogus to shift left orerand in c << for string representation of binary
 * number, performed in place */
void shiftLeft(char *bin, int shiftSize);
/* return the binary form of an ascii char represented by a string, returns
 * newly allocated space */
char *aToBin(char ascii);
/* adds the bin to translation with label as the assosiated lable to that
 * translation */
void addTranslation(char *bin, char *label, Translation *trans);
/* function for debugging */
void printTranslation(Translation *trans);
void updateTranslationAtIndex(Translation *trans, int index, char *bin,
                              char *label);
/* sets the provided ARE to bin */
void setARE(char *bin, enum ARE are);
/* returns newly allocated string representing the base 32 translation of num */
char *intToBase32(int num);
/* preforms bitwise OR between targetBin and operandBin the result is stored in
 * targetBin */
void orInPlace(char *targetBin, char *operandBin);

#endif
