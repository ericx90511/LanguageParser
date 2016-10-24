#ifndef SCANNER_H
#define SCANNER_H

#include <regex.h>
#include "regex.h"
#include <string>
#include <map>

class Token ;

/* This enumerated type is used to keep track of what kind of
   construct was matched. 
*/
enum tokenEnumType { 

    // Keywords 22
    letKwd, inKwd, endKwd, ifKwd, thenKwd, elseKwd, 
    printKwd, readKwd, writeKwd, 
    integerKwd, floatKwd, booleanKwd, stringKwd, 
    trueKwd, falseKwd, 

    headKwd, tailKwd, nullKwd, 
    mapKwd, filterKwd, foldKwd, 
    zipKwd,

    // Constants 3
    intConst, floatConst, stringConst, 

    // Names 1
    variableName ,

    // Punctuation 25+2
    leftParen, rightParen, 
    leftSquare, rightSquare, leftAngle, rightAngle,
    colon, colonColon, comma, semiColon, 
    equalsSign, 

    plusSign, star, dash, forwardSlash, percent, 
    backSlash, arrow, 
    equalsEquals, 
    lessThanEquals, greaterThanEquals, notEquals, 
    plusPlus, dotDot, 
    tupleOp,

    endOfFile ,
    lexicalError
} ;
typedef enum tokenEnumType terminal_t ;

// Below you need to write your class declarations for Token and Scanner.
class Token {
public:
	Token();
	Token(const char*, terminal_t, Token*);
        terminal_t terminal;
        std::string lexeme;
        Token* next;
};
class Scanner {
public:
	Scanner();
	std::map<int, regex_t*> map;
	Token *scan (const char *) ;
	regex_t *whiteSpace;
	regex_t *blockComment;
	regex_t *lineComment;
};
#endif /* SCANNER_H */
