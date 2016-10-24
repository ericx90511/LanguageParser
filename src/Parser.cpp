/* A recursive descent parser with operator precedence.

   Author: Eric Van Wyk

   This algorithms is based on the work of Douglas Crockford in "Top
   Down Operator Precedence", a chapter in the book "Beautiful Code".
   Mr. Crockford describes in his chapter how his work comes from that
   of Based on algorithms described Vaughan Pratt in "Top Down Operator
   Precedence", presented at the ACM Symposium on Principles of
   Programming Languages.

   Douglas Crockford's chapter is available at 
    http://javascript.crockford.com/tdop/tdop.html

   Vaughan Pratt's paper is available at 
    http://portal.acm.org/citation.cfm?id=512931

   These are both quite interesting works and worth reading if you
   find the problem of parsing to be an interesting one.

   Last modified: March 4, 2014.
*/

#include "Parser.h"
#include "scanner.h"
#include "ExtToken.h"
#include "AST.h"

#include <assert.h>

using namespace std ;

Parser::Parser ( ) { } 

ParseResult Parser::parse (const char *text) {
    assert (text != NULL) ;

    ParseResult pr ;
    try {
        Scanner *s = new Scanner() ;
        tokens = extendTokenList ( this, s->scan (text) ) ;

        assert (tokens != NULL) ;
        currToken = tokens ;
        pr = parseProgram( ) ;
    }
    catch (string errMsg) {
        pr.ok = false ;
        pr.errors = errMsg ;
        pr.ast = NULL ;
    }
    return pr ;
}

/* 
 * parse methods for non-terminal symbols
 * --------------------------------------
 */
/*

for pr.ast = new classname(parameter)
read AST files and ExtToken files

*/
// Program DONE
ParseResult Parser::parseProgram () {
    ParseResult pr ;
	pr.ast = NULL;
    // Program ::= DeclList
    ParseResult temp =parseDeclList() ;
    DeclList *e1=dynamic_cast<DeclList *>(temp.ast);
    // verify that ast field is a DeclList pointer

    match(endOfFile) ;
    pr.ast = new Program(e1);
    return pr ;
}

// DeclList DONE
ParseResult Parser::parseDeclList () {
    ParseResult pr ;
    // DeclList ::= Decl
    //initialize string for DeclList
    std::string o="";
    ParseResult temp = parseDecl() ;
    /*! dynamic cast for type conversion */
    Decl *e1=dynamic_cast<Decl *>(temp.ast);
    DeclList *e2;
    // verify that ast field is a Declt pointer

    if ( nextIs(variableName) ) {
        // DeclList ::= Decl DeclList
        /*! string o for space in DeclList*/
        o=" ";
        ParseResult temp2 = parseDeclList() ;
        /*! dynamic cast for type conversion */
        e2=dynamic_cast<DeclList *>(temp2.ast);
        // verify that ast field is a DeclList pointer
    }
    else {
		//emptyList
        e2=new EmptyDecl();
    }
    pr.ast = new DeclList(e1,o,e2);
    return pr ;
}

// Decl DONE
ParseResult Parser::parseDecl () {
    ParseResult pr ;
    /*! Decl ::= variableName '::' TypeExpr '=' Expr ';'*/

    match(variableName) ;
    /*! lexeme for variablename*/
    VariableName *e1=new VariableName(prevToken->lexeme);
    match(colonColon) ;

    ParseResult temp=parseTypeExpr() ;
    /*! dynamic cast for type conversion */
    TypeExpr *e2=dynamic_cast<TypeExpr *>(temp.ast);
    /*! verify that ast field is a TypeExpr pointer*/

    match(equalsSign) ;
    ParseResult temp2=parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e3=dynamic_cast<Expr *>(temp2.ast);

    match(semiColon) ;
    pr.ast = new Decl(e1,e2,e3);
    return pr ;
}

// TypeExpr DONE
ParseResult Parser::parseTypeExpr () {
    ParseResult pr ;


    std::string o;
    if ( attemptMatch(integerKwd) ) { 
        // TypeExpr ::= 'Integer' DONE
        pr.ast= new TypeExprKwd(prevToken->lexeme);
    }

    else if ( attemptMatch(floatKwd) ) {
        // TypeExpr ::= 'Float' DONE
        pr.ast= new TypeExprKwd(prevToken->lexeme);
    }

    else if ( attemptMatch(stringKwd) ) {
        // TypeExpr ::= 'String' DONE
       pr.ast= new TypeExprKwd(prevToken->lexeme);
    }

    else if ( attemptMatch(booleanKwd) ) {
        // TypeExpr ::= 'Boolean' DONE
        pr.ast= new TypeExprKwd(prevToken->lexeme);
    }

    else if ( attemptMatch(leftSquare) ) {
        // TypeExpr ::= '[' TypeExpr ']' DONE

        ParseResult temp = parseTypeExpr() ;
        
        match(rightSquare) ;
        TypeExpr *e1=dynamic_cast<TypeExpr *>(temp.ast);
        pr.ast = new TypeExprBracket(e1);
    }

    else if ( attemptMatch(leftParen) ) {
        if ( nextIs(variableName) ) {
            // TypeExpr ::= '(' TypeBindingList ')' '->' TypeExpr DONE

            ParseResult temp2 = parseTypeBindingList() ;
            TypeBindingList* e1=dynamic_cast<TypeBindingList *>(temp2.ast);

            match(rightParen) ;
            
            match(arrow) ;
            //string for arrow
            std::string o = prevToken->lexeme;
            ParseResult temp3 = parseTypeExpr() ;
            /*! dynamic cast for type conversion */
            TypeExpr *e2=dynamic_cast<TypeExpr *>(temp3.ast);
            pr.ast = new TypeExprBinType(e1,o,e2);
            
        } else {
            // TypeExpr ::= '(' TypeExprList ')'  DONE

            /*!for case of TypeExprParen*/
            ParseResult temp4 = parseTypeExprList() ;
            match(rightParen) ;
            /*! dynamic cast for type conversion */
            TypeExprList *e3=dynamic_cast<TypeExprList *>(temp4.ast);

            pr.ast = new TypeExprParen(e3);
        }
    }
    else {
        throw ( makeErrorMsg ( currToken->terminal ) ) ;
    }
    
    return pr ;
}

// TypeExprList DONE
ParseResult Parser::parseTypeExprList () {
    ParseResult pr ;
    // TypeExprList ::= TypeExpr

    ParseResult temp = parseTypeExpr() ;
    /*! dynamic cast for type conversion */
    TypeExpr *e1 = dynamic_cast<TypeExpr *>(temp.ast);
    /*!initialze variables*/
    TypeExprList *e2;
    std::string o="";
    if ( attemptMatch(comma) ) {
        // TypeExprList ::= TypeExpr ',' TypeExprList
        o=", ";

        ParseResult temp2 = parseTypeExprList () ;
        /*! dynamic cast for type conversion */
        e2= dynamic_cast<TypeExprList *>(temp2.ast);
    }
    else {
       //no TypeExprs follow the one matched previously in this function. 
        e2=new EmptyTypeExprList();
    }    
    
    pr.ast = new TypeExprList(e1,o,e2);
    return pr ;
}


// TypeBindingList DONE
ParseResult Parser::parseTypeBindingList () {
    ParseResult pr ;

    // TypeBindingList ::= TypeBinding

    ParseResult temp = parseTypeBinding () ;
    /*! dynamic cast for type conversion */
    TypeBinding *e1 = dynamic_cast<TypeBinding *>(temp.ast);
    /*!initialze variables*/
    TypeBindingList *e2;
    std::string o= "";

    if ( attemptMatch(comma) ) {
        // TypeBindingList ::= TypeBinding ',' TypeBindingList
        o=", ";
        ParseResult temp2 = parseTypeBindingList () ;
        /*! dynamic cast for type conversion */
        e2 = dynamic_cast<TypeBindingList *>(temp2.ast);
    }
    else {
        //case of end
        e2= new EmptyTypeBindingList();
    }
    pr.ast = new TypeBindingList(e1,o,e2);
    return pr ;
}

// TypeBinding DONE
ParseResult Parser::parseTypeBinding () {
    ParseResult pr ;
    std::string o;
    // TypeBinding ::= variableName '::' TypeExpr
    match(variableName) ;
    /*! lexeme for variable*/
    VariableName *e1= new VariableName(prevToken->lexeme);

    match(colonColon) ;

    ParseResult temp =parseTypeExpr () ;
    /*! dynamic cast for type conversion */
    TypeExpr *e2 = dynamic_cast<TypeExpr *>(temp.ast);
    pr.ast = new TypeBinding (e1,e2);
    return pr ;
}

// OptionalExprList DONE
ParseResult Parser::parseOptionalExprList () {
    ParseResult pr ;
    if ( nextIs(rightSquare) || nextIs(rightParen) ) {
        // OptionalExprList ::= <<empty>>
        pr.ast = new OptionalEmpty();
    } else {
        // OptionalExprList ::= ExprList
        ParseResult temp = parseExprList() ;
        /*! dynamic cast for type conversion */
        ExprList *e1 = dynamic_cast<ExprList *>(temp.ast);
        pr.ast = new OptionalExprList(e1);
    }
    return pr ;
}

// ExprList  DONE
ParseResult Parser::parseExprList () {
    ParseResult pr ;
    // ExprList ::= Expr

    ParseResult temp =parseExpr(0) ;
    Expr *e1= dynamic_cast<Expr *>(temp.ast);
    /*!initialze variables*/
    ExprList *e2;
    std::string o="";

    if ( attemptMatch(comma) ) {
    // ExprList ::= Expr ',' ExprList
    o = ", ";
    ParseResult temp2 = parseExprList() ;
    /*! dynamic cast for type conversion */
    e2=dynamic_cast<ExprList *>(temp2.ast);
    
    }
    else {
		//empty listExprs

        // for empty ExprList
    o= " ";
    e2= new ExprEmpty();
    }

    pr.ast = new ExprList(e1,o,e2);
    return pr ;
}

// Expr
ParseResult Parser::parseExpr (int rbp) {
    /* Examine current token, without consuming it, to call its
       associated parse methods.  The ExtToken objects have 'nud' and
       'led' methods that are dispatchers that call the appropriate
       parse methods.*/
    ParseResult left = currToken->nud() ;
   
    while (rbp < currToken->lbp() ) {
        left = currToken->led(left) ;
    }


    return left ;
}

/*
 * parse methods for productions
 * -----------------------------
 */

// Expr ::= variableName DONE
ParseResult Parser::parseVariableName ( ) {
    ParseResult pr ;
    match ( variableName ) ;
    /*! capture lexeme */
    pr.ast=new VariableName(prevToken->lexeme);
    return pr ;
}

// Expr ::= intConst DONE
ParseResult Parser::parseIntConst ( ) {
    ParseResult pr ;
    match ( intConst ) ;
    /*! capture lexeme */
    pr.ast= new IntConst(prevToken->lexeme);
    return pr ;
}

// Expr ::= floatConst DONE
ParseResult Parser::parseFloatConst ( ) {
    ParseResult pr ;
    match ( floatConst ) ;
    /*! capture lexeme */
    pr.ast= new FloatConst(prevToken->lexeme);
    return pr ;
}

// Expr ::= stringConst DONE
ParseResult Parser::parseStringConst ( ) {
    ParseResult pr ;
    match ( stringConst ) ;
    /*! capture lexeme */
    pr.ast= new StringConst(prevToken->lexeme);
    return pr ;
}

// Expr ::= trueKwd DONE
ParseResult Parser::parseTrueKwd ( ) {
    ParseResult pr ;
    match ( trueKwd ) ;
    /*! capture lexeme */
    pr.ast= new TrueKwd(prevToken->lexeme);
    return pr ;
}

// Expr ::= falseKwd DONE
ParseResult Parser::parseFalseKwd ( ) {
    ParseResult pr ;
    match ( falseKwd ) ;
    /*! capture lexeme */
    pr.ast= new FalseKwd(prevToken->lexeme);
    return pr ;
}


ParseResult Parser::parseNestedOrTupleExpr ( ) {
    ParseResult pr ;
    match ( leftParen ) ;
    // Expr ::= '(' Expr ')'  DONE
    ParseResult temp = parseExpr(0) ; 
    Expr *e1=dynamic_cast<Expr *>(temp.ast);
    /*!initialze variables*/
    ExprList *e2;
    std::string o="";
    if ( attemptMatch(comma) ) {
        // Expr ::= '(' Expr ',' ExprList ')'  DONE

        /*! capture lexeme */
        o=prevToken->lexeme;
        ParseResult temp2 = parseExprList();
        /*! dynamic cast for type conversion */
        e2=dynamic_cast<ExprList *>(temp2.ast);
    }
    else {
		//no ExprList to parse 

        //empty class for NestedOrTupleExpr
        e2=new ExprEmpty();
    }
    match(rightParen) ;

    pr.ast = new NestedOrTupleExpr(e1,o,e2);
    return pr ;
}

// Expr ::= '[' ExprList ']'  Original documentation
// Expr ::= '[' OptionalExprList ']'  Original implementation

// Expr ::= '[' OptionalExprList ']'  before adding TypeExpr to literals
//       |  '[' Expr '..' Expr ']'     

// Expr ::= '[' '<' TypeExpr '>' OptionalExprList ']'  after TypeExpr
//       |  '[' Expr '..' Expr ']'     

ParseResult Parser::parseBraketListExpr ( ) {
    ParseResult pr ;
    match ( leftSquare ) ;

    if ( attemptMatch(leftAngle) ) {

        ParseResult temp=parseTypeExpr() ;
        /*! dynamic cast for type conversion */
        TypeExpr *e1=dynamic_cast<TypeExpr *>(temp.ast);
        match(rightAngle) ;
   
        ParseResult temp2=parseOptionalExprList() ; 
        /*! dynamic cast for type conversion */
        OptionalExprList *e2=dynamic_cast<OptionalExprList *>(temp2.ast);
        pr.ast = new TypeOpt(e1,e2);
    }
    else {
        ParseResult temp3=parseExpr(0);
        /*! dynamic cast for type conversion */
        Expr *e3=dynamic_cast<Expr *>(temp3.ast); 
        match(dotDot) ; 
        ParseResult temp4=parseExpr(0); 
        /*! dynamic cast for type conversion */
        Expr *e4=dynamic_cast<Expr *>(temp4.ast);
        pr.ast = new TwoExpr(e3,e4);
    }
    match(rightSquare) ;
    return pr ;
}

// Expr ::= variableName '(' OptionnalExprList ')'  DONE
ParseResult Parser::parseFunctionCall ( ParseResult left ) {
    // parser has already matched variableName on left.ast


    ParseResult pr ;
    /*! dynamic cast for type conversion */
    VariableName *e1=dynamic_cast<VariableName *>(left.ast);
    match ( leftParen ) ;

    ParseResult temp=parseOptionalExprList() ; 
    /*! dynamic cast for type conversion */
    OptionalExprList *e2=dynamic_cast<OptionalExprList *>(temp.ast);
    match(rightParen) ;
    pr.ast=new FunctionCall(e2,e1);
    return pr ;
}


// Expr ::= Expr '+' Expr  DONE
ParseResult Parser::parseAddition ( ParseResult left ) {
    // parser has already matched left expression 
    ParseResult pr ;

    match(plusSign) ;

    std::string oprat = prevToken->lexeme;
    
    ParseResult right = parseExpr( prevToken->lbp() ); 
    
    Expr *e1 = dynamic_cast<Expr *>(left.ast);
    Expr *e2 = dynamic_cast<Expr *>(right.ast);
    pr.ast = new PlusExpr (e1,oprat, e2) ;

    return pr ;
}

// Expr ::= Expr '*' Expr  DONE
ParseResult Parser::parseMultiplication ( ParseResult left ) {
    // parser has already matched left expression 

    ParseResult pr ;

    match ( star ) ;
    std::string oprat = prevToken->lexeme;

    ParseResult right = parseExpr( prevToken->lbp() ); 

    Expr *e1 = dynamic_cast<Expr *>(left.ast);
    Expr *e2 = dynamic_cast<Expr *>(right.ast);
    pr.ast = new MulExpr(e1,oprat,e2);

    return pr ;
}
// Expr ::= Expr '-' Expr  DONE
ParseResult Parser::parseSubtraction ( ParseResult left ) {
    // parser has already matched left expression 

    ParseResult pr ;

    match ( dash ) ;
    std::string oprat = prevToken->lexeme;
    ParseResult right = parseExpr( prevToken->lbp() ); 

    Expr *e1 = dynamic_cast<Expr *>(left.ast);
    Expr *e2= dynamic_cast<Expr *>(right.ast);
    pr.ast = new MinusExpr(e1,oprat,e2);
    return pr ;
}
// Expr ::= Expr '/' Expr  DONE
ParseResult Parser::parseDivision ( ParseResult left ) {
    // parser has already matched left expression 
    ParseResult pr ;

    match ( forwardSlash ) ;
    std::string oprat = prevToken->lexeme;
    ParseResult right =parseExpr( prevToken->lbp() ); 

    Expr *e1= dynamic_cast<Expr *>(left.ast);
    Expr *e2 = dynamic_cast<Expr *>(right.ast);
    pr.ast= new DivExpr(e1,oprat,e2);

    return pr ;
}
// Expr ::= Expr '%' Expr  DONE
ParseResult Parser::parseModulus ( ParseResult left ) {
    // parser has already matched left expression 

    ParseResult pr ;

    match ( percent ) ;
    std::string oprat = prevToken->lexeme;

    ParseResult right = parseExpr( prevToken->lbp() ); 
    /*! dynamic cast for type conversion */
    //parseResult left
    Expr *e1 = dynamic_cast<Expr *>(left.ast);
    Expr *e2 = dynamic_cast<Expr *>(right.ast);
    pr.ast = new MouExpr(e1,oprat,e2);
    return pr ;
}

// Expr ::= Expr '==' Expr  DONE
// Expr ::= Expr '<' Expr
// Expr ::= Expr '>' Expr
// Expr ::= Expr '>=' Expr
// Expr ::= Expr '<=' Expr
// Expr ::= Expr '!=' Expr
/* Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
 */
ParseResult Parser::parseRelationalExpr ( ParseResult left ) {
    // parser has already matched left expression 

    // terminal_t termin=currToken->terminal;
    ParseResult pr ;

    

    nextToken( ) ;
    std::string o =prevToken->lexeme;
    // just advance token, since examining it in parseExpr led 
    // to this method being called.

    ParseResult temp=parseExpr( prevToken->lbp() ); 
    /*! dynamic cast for type conversion */
    //ParseResult left
    Expr *e1=dynamic_cast<Expr *>(left.ast);
    Expr *e2=dynamic_cast<Expr *>(temp.ast);
    
    pr.ast = new BinOp (e1,o,e2);
    return pr ;
}

// Expr ::= Expr ':' Expr  DONE
ParseResult Parser::parseConsExpr ( ParseResult left ) {
    // parser has already matched left expression 

    ParseResult pr ;
    Expr *e1=dynamic_cast<Expr *>(left.ast);
    match(colon) ; 
    /*! capture lexeme */
    std::string o=prevToken->lexeme;
    ParseResult temp=parseExpr( prevToken->lbp() ); 
    /*! dynamic cast for type conversion */
    Expr *e2=dynamic_cast<Expr *>(temp.ast);

    pr.ast=new ConsExpr(e1,o,e2);
    return pr ;
}

// Expr ::= Expr '++' Expr  DONE
ParseResult Parser::parseAppendExpr ( ParseResult left ) {
    // parser has already matched left expression 

    ParseResult pr ;
    Expr *e1=dynamic_cast<Expr *>(left.ast);
    match(plusPlus) ; 
    //capture lexme
    std::string o=prevToken->lexeme;
    ParseResult temp =parseExpr( prevToken->lbp() ); 
    /*! dynamic cast for type conversion */
    Expr *e2=dynamic_cast<Expr *>(temp.ast);
    pr.ast=new AppendExpr(e1,o,e2);
    return pr ;
}
/*
// Expr ::= '[' Expr '..' Expr ']'  DONE
ParseResult Parser::parseDotDotExpr ( ParseResult left ) {
    // parser has already matched left expression 
    Expr *e1 = dynamic_cast<Expr *>(left.ast) ;
    if ( ! e1 ) throw ( (string) "Bad cast of first Expr in ListRange" ) ;

    ParseResult pr ;

    match(dotDot) ; 

    ParseResult prExpr = parseExpr( prevToken->lbp() ); 

    Expr *e2 = dynamic_cast<Expr *>(prExpr.ast) ;
    if ( ! e2 ) throw ( (string) "Bad cast of second Expr in ListRange" ) ;
//    throw ((string) "IN LIST RANGE");
    pr.ast = new ListRange (e1, e2) ;
    return pr ;
}
*/

/*! Expr ::= 'let' DeclList 'in' Expr 'end'  DONE */
ParseResult Parser::parseLetExpr ( ) {
    ParseResult pr ;
    match(letKwd) ; 

    ParseResult temp =parseDeclList() ;
    /*! dynamic cast for type conversion*/
    DeclList *e1=dynamic_cast<DeclList *>(temp.ast);

    match(inKwd) ; 

    ParseResult temp2= parseExpr(0); 
    /*!dynamic cast for type conversion */
    Expr *e2=dynamic_cast<Expr *>(temp2.ast);
    match(endKwd) ; 
    pr.ast=new LetExpr(e1,e2);
    return pr ;
}

/*! Expr ::= 'if' Expr 'then' Expr 'else' Expr  DONE */
ParseResult Parser::parseIfThenElse ( ) {
    ParseResult pr ;
    match(ifKwd) ;

    ParseResult temp = parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e1 = dynamic_cast<Expr *>(temp.ast);
    match(thenKwd) ;

    ParseResult temp2 = parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e2 = dynamic_cast<Expr *>(temp2.ast);
    match(elseKwd);

    ParseResult temp3 = parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e3 = dynamic_cast<Expr *>(temp3.ast);

    pr.ast = new IfThenElse(e1,e2,e3); 
    return pr ;
}

// Expr ::= 'print' '(' Expr ')'  DONE
ParseResult Parser::parsePrintExpr ( ) {
    ParseResult pr ;
    match(printKwd) ;
    /*! capture lexeme */
    std::string o= prevToken->lexeme;

    match(leftParen) ;

    ParseResult temp =parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e1=dynamic_cast<Expr *>(temp.ast);
    match(rightParen) ;
    
    pr.ast = new PrintExpr(e1, o);

    return pr ;
}

// Expr ::= 'write' '(' Expr, Expr ')'  
ParseResult Parser::parseWriteExpr ( ) {
    ParseResult pr ;
    match(writeKwd) ;
    /*! capture lexeme */
    std::string o=prevToken->lexeme;
    match(leftParen) ;

    ParseResult temp =parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e1 = dynamic_cast<Expr *>(temp.ast);
    match(comma) ;

    ParseResult temp2 = parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e2 = dynamic_cast<Expr *>(temp2.ast);
    match(rightParen) ;

    pr.ast = new WriteExpr(e1, o ,e2);
    return pr ;
}

// Expr ::= 'read' '<' TypeExpr '>' '(' Expr ')'
ParseResult Parser::parseReadExpr ( ) {
    ParseResult pr ;
    match(readKwd) ;
    /*! capture lexeme */
    std::string o = prevToken->lexeme;
    match(leftAngle) ;

    ParseResult temp = parseTypeExpr() ;
    /*! dynamic cast for type conversion */
    TypeExpr *e1 = dynamic_cast<TypeExpr *>(temp.ast);
    match(rightAngle) ;
    match(leftParen) ;

    ParseResult temp2 =parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e2 = dynamic_cast<Expr *>(temp2.ast);

    match(rightParen) ;
    pr.ast = new ReadExpr(e1,o,e2);
    return pr ;
}

// Expr ::= 'head' '(' Expr ')'
ParseResult Parser::parseHeadExpr ( ) {
    ParseResult pr ;
    match(headKwd) ;
    /*! capture lexeme */
    std::string o = prevToken->lexeme;
    match(leftParen) ;

    ParseResult temp = parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e1 = dynamic_cast<Expr *>(temp.ast);
    match(rightParen) ;

    pr.ast = new HeadExpr(e1, o);
    return pr ;
}

// Expr ::= 'tail' '(' Expr ')'
ParseResult Parser::parseTailExpr ( ) {
    ParseResult pr ;
    match(tailKwd) ;
    /*! capture lexeme */
    std::string o = prevToken->lexeme;
    match(leftParen) ;

    ParseResult temp = parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e1 = dynamic_cast<Expr *>(temp.ast);
    match(rightParen) ;

    pr.ast = new TailExpr(e1, o);
    return pr ;
}

// Expr ::= 'null' '(' Expr ')'
ParseResult Parser::parseNullExpr ( ) {
    ParseResult pr ;
    match(nullKwd) ;
    /*! capture lexeme */
    std::string o= prevToken->lexeme;
    match(leftParen) ;

    ParseResult temp =parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e1 = dynamic_cast<Expr *>(temp.ast);
    match(rightParen) ;

    pr.ast = new NullExpr(e1,o);
    return pr ;
}

// Expr ::= 'map' '(' Expr ',' Expr ')'
ParseResult Parser::parseMapExpr ( ) {
    ParseResult pr ;

    match(mapKwd) ;
    /*! capture lexeme */
    std::string o = prevToken->lexeme;
    match(leftParen) ;

    ParseResult temp = parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e1= dynamic_cast<Expr *>(temp.ast);
    match(comma) ;

    ParseResult temp2 = parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e2 = dynamic_cast<Expr *>(temp2.ast);
    match(rightParen) ;

    pr.ast = new MapExpr(e1,o,e2);
    return pr ;
}

// Expr ::= 'filter' '(' Expr ',' Expr ')'
ParseResult Parser::parseFilterExpr ( ) {
    ParseResult pr ;

    match(filterKwd) ;
    /*! capture lexeme */
    std::string o = prevToken->lexeme;
    match(leftParen) ;

    ParseResult temp = parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e1 = dynamic_cast<Expr *>(temp.ast);
    match(comma) ;

    ParseResult temp2 =parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e2 = dynamic_cast<Expr *>(temp2.ast);
    match(rightParen) ;

    pr.ast = new FilterExpr(e1,o,e2);
    return pr ;
}

// Expr ::= 'fold' '(' Expr ',' Expr ',' Expr ')'
ParseResult Parser::parseFoldExpr ( ) {
    ParseResult pr ;

    match(foldKwd) ;
    /*! capture lexeme */
    std::string o = prevToken->lexeme;
    match(leftParen) ;

    ParseResult temp = parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e1 = dynamic_cast<Expr *>(temp.ast);
    match(comma) ;

    ParseResult temp2 = parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e2 = dynamic_cast<Expr *>(temp2.ast);
    match(comma) ;

    ParseResult temp3 =parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e3 = dynamic_cast<Expr *>(temp3.ast);
    match(rightParen) ;

    pr.ast = new FoldExpr(e1,o,e2,e3);
    return pr ;
}

// Expr ::= 'zip' '(' Expr ',' Expr ')'
ParseResult Parser::parseZipExpr ( ) {
    ParseResult pr ;

    match(zipKwd) ;
    /*! capture lexeme */
    std::string o = prevToken->lexeme;
    match(leftParen) ;

    ParseResult temp = parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e1 = dynamic_cast<Expr *>(temp.ast);
    match(comma) ;

    ParseResult temp2 = parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e2 = dynamic_cast<Expr *>(temp2.ast);
    match(rightParen) ;

    pr.ast = new ZipExpr(e1,o,e2);
    return pr ;
}

//Expr ::= '\' TypeBindingList '->' Expr
ParseResult Parser::parseLambdaExpr ( ) {
    ParseResult pr ;
    match(backSlash) ;

    ParseResult temp = parseTypeBindingList() ;
    /*! dynamic cast for type conversion */
    TypeBindingList *e1=dynamic_cast<TypeBindingList *>(temp.ast);
    match(arrow) ;
    /*! capture lexeme */
    std::string o = prevToken->lexeme;
    ParseResult temp2 = parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e2=dynamic_cast<Expr *>(temp2.ast);
    pr.ast = new LambdaExpr(e1,o,e2);
    return pr ;
}

// Expr ::= tupleOp '(' Expr ')'
ParseResult Parser::parseProjectionExpr () {
    ParseResult pr ;

    match(tupleOp) ;
    ExtToken *op = prevToken ;
    /*! capture lexeme */
    std::string o=op->lexeme;
    // ExprList *e1=dynamic_cast<ExprList *>(op);
    match(leftParen) ;

    ParseResult temp = parseExpr(0) ;
    /*! dynamic cast for type conversion */
    Expr *e2 = dynamic_cast<Expr *>(temp.ast);
    match(rightParen) ;

    pr.ast = new ProjectionExpr(e2, o);
    return pr ;
}


// Helper function used by the parser.

void Parser::match (terminal_t tt) {
    if (! attemptMatch(tt)) {
        throw ( makeErrorMsgExpected ( tt ) ) ;
    }
}

bool Parser::attemptMatch (terminal_t tt) {
    if (currToken->terminal == tt) { 
        nextToken() ;
        return true ;
    }
    return false ;
}

bool Parser::nextIs (terminal_t tt) {
    return currToken->terminal == tt ;
}

void Parser::nextToken () {
    if (currToken->terminal == endOfFile && currToken->next == NULL) {
        prevToken = currToken ;
    } else if (currToken->terminal != endOfFile && currToken->next == NULL) {
        throw ( makeErrorMsg ( "Error: tokens end with endOfFile" ) ) ;
    } else {
        prevToken = currToken ;
        currToken = currToken->next ;
    }
}

string Parser::terminalDescription ( terminal_t terminal ) {
    Token *dummyToken = new Token ("",terminal, NULL) ;
    ExtToken *dummyExtToken = extendToken (this, dummyToken) ;
    string s = dummyExtToken->description() ;
    delete dummyToken ;
    delete dummyExtToken ;
    return s ;
}

string Parser::makeErrorMsgExpected ( terminal_t terminal ) {
    string s = (string) "Expected " + terminalDescription (terminal) +
        " but found " + currToken->description() ;
    return s ;
}

string Parser::makeErrorMsg ( terminal_t terminal ) {
    string s = "Unexpected symbol " + terminalDescription (terminal) ;
    return s ;
}

string Parser::makeErrorMsg ( const char *msg ) {
    return msg ;
}

