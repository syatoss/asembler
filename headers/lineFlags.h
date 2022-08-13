#ifndef LINE_FLAGS_H
#define LINE_FLAGS_H

typedef struct {
  int hasLabelDef;
} LineFlags;

/* allocates new memmory for line flags and returns a pointer to it */
LineFlags *newLineFlags();
/* sets all flags to their default value */
void clearFlags(LineFlags *flags);
/* frees the pointer to flags and all of the assosiated allocated memory */
void freeLineFlags(LineFlags *flags);

#endif
