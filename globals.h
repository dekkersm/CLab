//
// Created by dekke on 05/08/2022.
//

#ifndef MAMAN14_GLOBALS_H
#define MAMAN14_GLOBALS_H

#define MAX_CHARS_IN_LINE 80
#define MAX_LINES_IN_MACRO 10
#define MAX_CHARS_IN_FILE_NAME 50
#define MAX_CHARS_IN_DATA_NUM 6 // max num represented by 10 bits is 4 digits long + sign + \0
#define SYMBOL_MAX_CHAR_LENGTH 30

#define FIRST_ADDRESS_IN_OBJ_FILE 100

// Offsets for memory words
#define OPCODE_WORD_OFFSET 6
#define SRC_OPERAND_WORD_OFFSET 4
#define DEST_OPERAND_WORD_OFFSET 2
#define ARE_WORD_OFFSET 0

/* !,@,#,$,%,^,&,*,<,>,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v */

#endif //MAMAN14_GLOBALS_H
