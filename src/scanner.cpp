/* This program counts words and numbers, except for those not found
   in comments.

   Author(s): Eric Van Wyk
*/
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include "regex.h"
#include "scanner.h"
#include <sys/stat.h>
#include <sys/types.h>  
#include <map>
#include <iostream>

using namespace std;



int consumeWhiteSpaceAndComments (regex_t *whiteSpace, 
                                  regex_t *blockComment,
                                  regex_t *lineComment,
                                  const char *text) ;


Token::Token() {
    terminal = lexicalError;
    next = NULL;
    lexeme = "";
    
}
Token::Token(const char* lex, terminal_t ter , Token* ne){
  lexeme= lex;
  next = ne;
  terminal = ter;
}
//Scanner Token Type
Token *Scanner::scan (const char *text) {
  
    int numMatchedChars = 0;
    int maxNumMatchedChars = 0 ;
    terminal_t index;
    
   
    //regular expression for consuming

    // For linked list

    Token *head = NULL;
    Token *nodePtr = NULL;
   
    bool tokenFound = false;
    terminal_t keyword = lexicalError;
    // Consume leading white space and comments
    numMatchedChars = consumeWhiteSpaceAndComments (whiteSpace, blockComment, lineComment, 
                                                    text) ;
    text = text + numMatchedChars ;
   
    //std::cout<<"test for first one let "<<map.at(letKwd)<<std::endl;
    //std::cout<<"test for first one "<<map.at(0)<<std::endl;
    
    
while ( text[0] != '\0' ) {

  
  Token *newNode = new Token();
  maxNumMatchedChars = 0 ;
  tokenFound = false;
  
  for(index = letKwd; index <= tupleOp; index= static_cast<terminal_t>(index + 1)){
    //std::cout<<"index value "<<index<<std::endl;
    //std::cout<<"regular epxression for index key word:  "<<map.at(index)<<std::endl;
    //Assign regular express at index to pointer regex
    regex_t *regex = map.at(index);

    //Match regex with text
    numMatchedChars = matchRegex(regex, text);
    
    //If match more than zero character, find the token
    if (numMatchedChars > maxNumMatchedChars){
      maxNumMatchedChars = numMatchedChars ;
      tokenFound = true;
      keyword= static_cast<terminal_t>(index);
      
    }
    
  }
      
  //token found, ready for linked list
  if (tokenFound){
    std::string lexeme(text,maxNumMatchedChars);
    newNode->lexeme = lexeme;
    newNode->terminal = (terminal_t)keyword;
    text = text +maxNumMatchedChars;
    
  }else{
    // match number test = test+1
    numMatchedChars = 1;
    newNode->terminal= lexicalError;
    std::string lexeme (text, numMatchedChars);
    newNode->lexeme = lexeme;
    text = text + 1 ;
  }
  if(!nodePtr){
    head = newNode;
  }
  else{
    nodePtr->next = newNode;
  }
  nodePtr = newNode;  
  // Consume white space and comments before trying again for
  // another word or integer.
  numMatchedChars = consumeWhiteSpaceAndComments (whiteSpace, blockComment, lineComment, 
						  text) ;
  text = text + numMatchedChars ;
 }
 Token *tail = new Token();
 tail->terminal = endOfFile;
 tail->lexeme = '\0';
 if(!nodePtr){
   head = tail;   
 }else{
   nodePtr->next = tail;
 }
 tail->next=NULL;
 return head;
}

Scanner::Scanner(){
    
  //Constructor
  map[letKwd] = makeRegex("^(let)");
  
  map[inKwd] = makeRegex ("^(in)");
  
  map[endKwd] = makeRegex ("^(end)");
  
  map[ifKwd] = makeRegex ("^(if)");
  
  map[thenKwd] = makeRegex ("^(then)");

  map[elseKwd] = makeRegex ("^(else)");
  
  map[printKwd] = makeRegex ("^(print)");
  
  map[readKwd] = makeRegex ("^(read)");
  
  map[writeKwd] = makeRegex ("^(write)");
  
  map[integerKwd] = makeRegex ("^(Integer)");
  
  map[floatKwd] = makeRegex ("^(Float)");

  map[booleanKwd] = makeRegex ("^(Boolean)");
  
  map[stringKwd] = makeRegex ("^(String)");
  
  map[trueKwd] = makeRegex ("^(true)");
  
  map[falseKwd] = makeRegex ("^(false)");
  
  map[headKwd] = makeRegex ("^(head)");
  
  map[tailKwd] = makeRegex ("^(tail)");
  
  map[nullKwd] = makeRegex ("^(null)");
    
  map[mapKwd] = makeRegex ("^(map)");
  
  map[filterKwd] = makeRegex ("^(filter)");
    
  map[foldKwd] = makeRegex ("^(fold)");
    
  map[zipKwd] = makeRegex ("^(zip)");


  //Constants

  map[intConst] = makeRegex ("^[0-9]+");

  map[floatConst] = makeRegex ("^[0-9]+\\.[0-9]+") ;

  map[stringConst] = makeRegex ("^\\\"([^\"\\\\]|\\\\.)*\\\"");

  //Names
 
  map[variableName] = makeRegex ("^[a-zA-Z]+([_]*[a-zA-Z0-9_])*");

  //Punctuations
 
  map[leftParen] = makeRegex("^\\(");

  map[rightParen] = makeRegex("^\\)");

  map[leftSquare] = makeRegex("^\\[");
    
  map[rightSquare] = makeRegex ("^\\]");

  map[leftAngle] = makeRegex ("^<");

  map[rightAngle] = makeRegex ("^>");

  map[colon] = makeRegex ("^:");

  map[colonColon] = makeRegex ("^::");

  map[comma] = makeRegex ("^,");

  map[semiColon] = makeRegex ("^;");

  map[equalsSign] = makeRegex ("^=");

  map[plusSign] = makeRegex ("^\\+");
 
  map[star] = makeRegex ("^\\*"); 

  map[dash] = makeRegex ("^\\-");

  map[forwardSlash]= makeRegex ("^\\/");

  map[percent] = makeRegex ("^%");

  map[backSlash] = makeRegex ("^[\\]");

  map[arrow] = makeRegex ("^\\->");

  map[equalsEquals] = makeRegex ("^=="); 
  
  map[lessThanEquals] = makeRegex ("^<=");

  map[greaterThanEquals] = makeRegex ("^>=");

  map[notEquals] = makeRegex ("^!=");
 
  map[plusPlus] = makeRegex ("^\\+\\+");

  map[dotDot] = makeRegex ("^\\.\\.");
 
  map[tupleOp] = makeRegex ("^\\#[1-9]+[0-9]*");


	whiteSpace = makeRegex ("^[\n\t\r ]+") ;
	
    blockComment= makeRegex ("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");
    
    lineComment = makeRegex ("^//[^\n]*\n");

}

int consumeWhiteSpaceAndComments (regex_t *whiteSpace, 
                                  regex_t *blockComment, 
																	regex_t *lineComment,
                                  const char *text) {
    int numMatchedChars = 0 ;
    int totalNumMatchedChars = 0 ;
    int stillConsumingWhiteSpace ;

    do {
        stillConsumingWhiteSpace = 0 ;  // exit loop if not reset by a match

        // Try to match white space
        numMatchedChars = matchRegex (whiteSpace, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }

        // Try to match block comments
        numMatchedChars = matchRegex (blockComment, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }

        // Try to match single-line comments
        numMatchedChars = matchRegex (lineComment, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }
    }
    while ( stillConsumingWhiteSpace ) ;    

    return totalNumMatchedChars ;
}
