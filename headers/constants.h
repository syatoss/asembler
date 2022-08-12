#ifndef CONSTANTS_h
#define CONSTANTS_h

#define STRING_BUFFER_SIZE 255
#define MACRO_START "macro"
#define MACRO_END "endmacro"
#define true 1
#define false 0
#define COMMENT_LINE_TOKEN ';'
#define MAX_INT_DIGITS 20
#define ASM_FILE_SUFFIX ".as"
#define OBJECT_FILE_SUFFIX ".ob"
#define EXTERN_FILE_SUFFIX ".ext"
#define ENTRY_FILE_SUFFIX ".ent"
#define SPREAD_FILE_SUFFIX ".am"
#define WORD_SIZE 10
#define MAX_WORDS_PER_INSTRUCTION 5
#define START_ADDRESS 100
#define ENTRY_LABEL ".entry"
#define EXTERN_LABEL ".extern"
#define NUM_OF_RESERVED_WORDS 26
#define NUM_OF_INSTRUCTIONS 16
#define NUM_OF_ZERO_OPERAND_INSTRUCTIONS 2
#define NUM_OF_ONE_OPERAND_INSTRUCTIONS 9
#define NUM_OF_TWO_OPERAND_INSTRUCTIONS 5
#define NUM_OF_REGISTERS 8
#define NUM_OF_DATA_TYPES 3
#define NUM_OF_LABEL_STATUSES 2
#define INVALID -1

#endif
