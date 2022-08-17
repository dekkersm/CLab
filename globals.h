#ifndef MAMAN14_GLOBALS_H
#define MAMAN14_GLOBALS_H

#define MAX_CHARS_IN_LINE 80
#define MAX_LINES_IN_MACRO 10
#define MAX_CHARS_IN_FILE_NAME 50
#define MAX_CHARS_IN_DATA_NUM 6 /*max num represented by 10 bits is 4 digits long + sign + \0*/
#define SYMBOL_MAX_CHAR_LENGTH 30
#define BIT_MASK_FOR_WORD_SIZE 1023 /*10 lit bits*/

/*Memory globals*/
#define FIRST_ADDRESS_IN_OBJ_FILE 100
#define MEMORY_ARRAY_WORD_SIZE 256

/*Offsets for memory words*/
#define OPCODE_WORD_OFFSET 6
#define SRC_OPERAND_WORD_OFFSET 4
#define DEST_OPERAND_WORD_OFFSET 2
#define ARE_WORD_OFFSET 0
#define EXTRA_MEMORY_WORD_OFFSET 2
#define SRC_REGISTER_WORD_OFFSET 6
#define DEST_REGISTER_WORD_OFFSET 2

#endif
