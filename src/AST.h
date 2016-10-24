#ifndef Node_H
#define Node_H

#include <string>
#include <iostream> 
#include <list>

#include "scanner.h"

class Decl;
class DeclList;
class Expr;
class ExprList;
class OptionalExprList;
class VariableName;
class TypeExpr;
class TypeExprList;
class TypeBinding;
class TypeBindingList;

// Node
class Node 
{
	public:
		virtual std::string unparse ( ) = 0 ;
		virtual std::string cppCode ( ) = 0; //-TODO iter4 
		virtual ~Node() { }
};

//Expr ::=
class Expr:public Node
{
	public:
		std::string unparse()=0;
		std::string cppCode()=0;
		/* data */
};

class TypeExpr:public Node
{
public:
	TypeExpr(){};
	virtual ~TypeExpr(){};
	std::string unparse()=0;
	std::string cppCode()=0;
	/* data */
};

/*! Program ::= DeclList
	Program class for DeclList
*/
class Program:public Node
{
public:
	/*! Program constructor.
		\param DeclList the class DeclList
	*/	
	Program(DeclList *one):d1(one){};
	virtual ~Program(){};
	/*! virtual unparse for Program*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	DeclList *d1;
};

// DeclList ::= Decl
// DeclList ::= Decl DeclList
class DeclList:public Node
{
public:
	DeclList(){};
	/*! DeclList constructor.
		\param Decl for class Decl
		\param string for space
		\param DeclList for class DeclList
	*/	
	DeclList(Decl *one,std::string o, DeclList *two):d1(one),op(o),d2(two){};
	virtual ~DeclList(){};
	/*! virtual unparse for DeclList */
	std::string unparse();
	/* data */
	std::string cppCode();
private:
	Decl *d1;
	std::string op;
	DeclList *d2;
};

class EmptyDecl:public DeclList
{
public:
	/*! EmptyDecl constructor.
		no param
	*/
	EmptyDecl(){};
	virtual ~EmptyDecl(){};
	std::string unparse(){return "";}
	std::string cppCode(){return "";}
	/* data */
};
// Decl ::= variableName '::' TypeExpr '=' Expr ';'
class Decl:public Node
{
public:
	/*! Decl constructor
	\param VariableName for class VariableName
	\param TypeExpr for class TypeExpr
	\param Expr for class Expr
	*/ 
	Decl(VariableName *one, TypeExpr *two, Expr *three):d1(one),d2(two),d3(three){};
	virtual ~Decl(){};
	/*! virtual unparse for Decl */
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	VariableName *d1;
	TypeExpr *d2;
	Expr *d3;
};
// TypeExpr ::= 'Integer' 
// TypeExpr ::= 'Float' 
// TypeExpr ::= 'String' 
// TypeExpr ::= 'Boolean' 
class TypeExprKwd:public TypeExpr
{
public:
	/*! TypeExprKwd constructor.
	\param string for keywords
	*/
	TypeExprKwd(std::string o):op(o){};
	virtual ~TypeExprKwd(){};
	/*! virtual unparse for TypeExprKwd */
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	std::string op;
};

// TypeExpr ::= '[' TypeExpr ']' 
class TypeExprBracket:public TypeExpr
{
public:
	/*! TypeExprBracket constructor.
		\param TypeExpr for class TypeExpr
	*/
	TypeExprBracket(TypeExpr *one):d1(one){};
	virtual ~TypeExprBracket(){};
	/*! virtual unparse for TypeExpr */
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	TypeExpr *d1;
};

// TypeExpr ::= '(' TypeBindingList ')' '->' TypeExpr 
class TypeExprBinType:public TypeExpr
{
public:
	/*! TypeExprBinType constructor.
	\param TypeBindingList for class TypeBindingList
	\param string for arrow
	\param TypeEpxr for class TypeExpr
	*/
	TypeExprBinType(TypeBindingList *one, std::string o, TypeExpr *two):d1(one), op(o),d2(two){};
	virtual ~TypeExprBinType(){};
	/*! virtual unparse for TypeExpr */
	std::string unparse();
	std::string cppCode();
	TypeBindingList *getTypeBindingList(){return d1;}
	std::string getString(){return op;}
	TypeExpr *getTypeExpr(){return d2;}
	/* data */
private:
	TypeBindingList *d1;
	std::string op;
	TypeExpr *d2;
};

// TypeExpr ::= '(' TypeExprList ')'  
class TypeExprParen:public TypeExpr
{
public:
	/*! TypeExprParen constructor.
	\param TypeExprList for class TypeExprList
	*/
	TypeExprParen(){};
	TypeExprParen(TypeExprList *one):d1(one){};
	virtual ~TypeExprParen(){};
	/*! virtual unparse for TypeExpr */
	std::string unparse();
	std::string cppCode();

	/* data */
private:
	TypeExprList *d1;
};


// TypeExprList ::= TypeExpr
// TypeExprList ::= TypeExpr ',' TypeExprList
class TypeExprList:public Node
{
public:
	TypeExprList(){};
	/*! TypeExprList constructor.
	\param TypeExpr for class TypeExpr
	\param string for comma
	\param TypeExprList for class TypeExprList
	*/
	TypeExprList(TypeExpr *one,std::string o, TypeExprList *two): d1(one),op(o), d2(two){};
	virtual ~TypeExprList(){};
	/*! virtual unparse for TypeExprList */
	std::string unparse ();
	std::string cppCode();

	/* data */
private:
	TypeExpr *d1;
	std::string op;
	TypeExprList *d2;
};

class EmptyTypeExprList:public TypeExprList
{
public:
	/*! EmptyTypeExprList constructor.
	no param
	*/
	EmptyTypeExprList(){};
	virtual ~EmptyTypeExprList(){};
	/*! virtual unparse for EmptyTypeExprList */
	std::string unparse(){return "";}
	std::string cppCode(){return "";}
	/* data */
};

// TypeBindingList ::= TypeBinding
// TypeBindingList ::= TypeBinding ',' TypeBindingList
class TypeBindingList:public Node
{
public:
	TypeBindingList(){};
	/*! TypeBindingList constructor.
	\param TypeBinding for class TypeBinding
	\param string for comma
	\param TypeBindingList for class TypeBinding List
	*/ 
	TypeBindingList(TypeBinding *one, std::string o, TypeBindingList *two): d1(one),op(o), d2(two){};
	virtual ~TypeBindingList(){};
	/*! virtual unparse for TypeBindingList */
	std::string unparse ();
	std::string cppCode();
	/* data */
private:
	TypeBinding *d1;
	std::string op;
	TypeBindingList *d2;
};

class EmptyTypeBindingList:public TypeBindingList
{
public:
	/*! EmptyTypeBindingList constructor
	no param*/
	EmptyTypeBindingList(){};
	virtual ~EmptyTypeBindingList(){};
	/*! virtual unparse for EmptyTypeBindingList */
	std::string unparse(){return "";}
	std::string cppCode(){return "";}
	/* data */
};

// TypeBinding ::= variableName '::' TypeExpr
class TypeBinding:public Node
{
public:
	/*! TypeBinding constructor.
	\param VariableName for class VariableName
	\param TypeExpr for class TypeExpr
	*/
	TypeBinding(VariableName *one, TypeExpr *two):d1(one), d2(two){};
	virtual ~TypeBinding(){};
	/*! virtual unparse for TypeBinding */
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	VariableName *d1;
	TypeExpr *d2;
};

// OptionalExprList ::= <<empty>>
// OptionalExprList ::= ExprList
class OptionalExprList:public Node
{
public:
	OptionalExprList(){};
	/*! OptionalExprList
	\param ExprList for class ExprList*/
	OptionalExprList(ExprList *one):d1(one){};
	virtual ~OptionalExprList(){};
	/*! virtual unparse for OptionalExprList */
	std::string unparse();
	std::string cppCode();
	ExprList *getExprList(){return d1;}
	/* data */
private:
	ExprList *d1;
};

class OptionalEmpty:public OptionalExprList
{
public:
	/*! OptionalEmpty constructor.
	no param*/
	OptionalEmpty(){};
	virtual ~OptionalEmpty(){};
	/*! virtual unparse for optionalEmpty */
	std::string unparse(){return "";}
	std::string cppCode(){return "";}
	/* data */
};

//ExprList ::= Expr
//ExprList ::= Expr ',' ExprList
class ExprList:public Node
{
public:
	/*! ExprList constructor.
	\param Expr for class Expr
	\param string for comma
	\param ExprList for class ExprList
	*/
	ExprList(Expr *one, std::string o, ExprList *two):d1(one),op(o),d2(two){};
	
	virtual ~ExprList(){};
	ExprList(){};
	/*! virtual unparse for ExprList */
	std::string unparse();
	std::string cppCode();
	Expr *getExpr(){return d1;}
	std::string getString(){return op;}
	ExprList *getExprList(){return d2;}

	/* data */
private:
	Expr *d1;
	std::string op;
	ExprList *d2;
	
};

class ExprEmpty:public ExprList
{
public:
	/*! ExprEmpty
	no param
	*/
	ExprEmpty(){};
	virtual ~ExprEmpty(){};
	/*! virtual unparse for ExprEmpty */
	std::string unparse(){return "";}
	std::string cppCode(){return "";}
	/* data */
};

//Expr ::= variableName 
class VariableName: public Expr
{
public:
	/*! VariableName constructor
	\param string for variablename
	*/
	VariableName(std::string lexeme): var(lexeme){};
	virtual ~VariableName(){};
	/* data */
	/*! virtual unparse for VariableName */
	std::string unparse();
	std::string cppCode();
private:
	std::string var;
};

//Expr ::= intConst
class IntConst : public Expr
{
public:
	/*! IntConst constructor.
	\param string for integer constant*/
	IntConst(std::string lexeme): intVar(lexeme){};
	virtual ~IntConst(){};
		
	/* data */
	/*! virtual unparse for IntConst */
	std::string unparse();
	std::string cppCode();
private:
	std::string intVar;
};

//Expr ::= floatConst 
class FloatConst : public Expr
{
public:
	/*! FloatConst constructor
	\param string for float constant*/
	FloatConst(std::string lexeme): intVar(lexeme){};
	virtual ~FloatConst(){};
		
	/* data */
	/*! virtual unparse for FloatConst */
	std::string unparse(){return intVar;}
	std::string cppCode();
private:
	std::string intVar;
};

//Expr ::= stringConst 
class StringConst : public Expr
{
public:
	/*! StringConst constructor
	\param string for string 
	*/
	StringConst(std::string lexeme): stringCon(lexeme){};
	virtual ~StringConst(){};
		
	/* data */
	/*! virtual unparse for StringConst */
	std::string unparse(){return stringCon;}
	std::string cppCode(){return stringCon;}
private:
	std::string stringCon;
};

//Expr ::= trueKwd 
class TrueKwd : public Expr
{
public:
	/*! TrueKwd constructor
	\param string for True keyword
	*/
	TrueKwd(std::string lexeme): stringCon(lexeme){};
	virtual ~TrueKwd(){};
		
	/* data */
	/*! virtual unparse for tureKwd */
	std::string unparse(){return stringCon;}
	std::string cppCode();
private:
	std::string stringCon;
};

//Expr ::= falseKwd 
class FalseKwd : public Expr
{
public:
	/*! FalseKwd constructor
	\param string for False keyword*/
	FalseKwd(std::string lexeme): stringCon(lexeme){};
	virtual ~FalseKwd(){};
		
	/* data */
	/*! virtual unparse for FalseKwd */
	std::string unparse(){return stringCon;}
	std::string cppCode();
private:
	std::string stringCon;
};

// Expr ::= '(' Expr ')' 
// Expr ::= '(' Expr ',' ExprList ')' 
class NestedOrTupleExpr: public Expr
{
public:
	virtual ~NestedOrTupleExpr(){};
	/*! NestedOrTupleExpr constructor.
	\param Expr for class Expr
	\param string for comma
	\param ExprList for class ExprList 
	*/
	NestedOrTupleExpr(Expr *one, std::string o, ExprList *two):d1(one),op(o),d2(two){};
	/*! virtual unparse for NestedOrTupleExpr */
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *d1;
	std::string op;
	ExprList *d2;
	
};

  
// Expr ::= '[' '<' TypeExpr '>' OptionalExprList ']'  
class TypeOpt:public Expr
{
public:
	/*! TypeOpt constructor
	\param TypeExpr for class TypeExpr
	\param OptionalExprList for class OptionalExprList
	*/
	TypeOpt(TypeExpr *one,OptionalExprList *two):d1(one),d2(two){};
	virtual ~TypeOpt(){};
	/*! virtual unparse for TypeOpt */
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	TypeExpr *d1;
	OptionalExprList *d2;
};
// Expr ::=  '[' Expr '..' Expr ']'  
class TwoExpr:public Expr
{
public:
	/*! TwoExpr constructor
	\param Expr for class Expr
	\param Expr for class Expr
	*/
   	TwoExpr(Expr *one,Expr *two):d1(one),d2(two){};
   	virtual ~TwoExpr(){};
   	/*! virtual unparse for TwoExpr */
   std::string unparse();
   std::string cppCode();

   	/* data */
private:
	Expr *d1;
	Expr *d2;
};   
// Expr ::= variableName '(' OptionnalExprList ')' 
class FunctionCall:public Expr
{
public:
	/*! FunctionCall constructor
	\param OptionalExprList for class OptionalExprList
	\param VariableName for class VariableName
	*/
	FunctionCall(OptionalExprList *one, VariableName *two):d1(one),d2(two){};
	virtual ~FunctionCall(){};
	/*! virtual unparse for FunctionCall */
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	OptionalExprList *d1;
	VariableName *d2;
};

//class Binary op
class PlusExpr:public Expr
{
public:
	virtual ~PlusExpr(){};
	/*! PlusExpr constructor
	\param Expr for class Expr
	\param string for operator
	\param Expr for class Expr
	*/
	PlusExpr(Expr *l, std::string o, Expr *r):left(l),right(r), op(o) {};
	/*! virtual unparse for PlusExpr */
	std::string unparse();
	std::string cppCode();
private:
	Expr *left;
	Expr *right;
	std::string op;		
};

//Expr ::= Expr '*' Expr 
class MulExpr:public Expr
{
public:
	/*! MulExpr constructor
	\param Expr for class Expr
	\param string for operator
	\param Expr for class Expr
	*/
	MulExpr(Expr *l, std::string o, Expr *r):left(l),right(r),op(o){};
	virtual ~MulExpr(){};
	/*! virtual unparse for MulExpr */
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *left;
	Expr *right;
	std::string op;
};

//Expr ::= Expr '-' Expr
class MinusExpr:public Expr
{
public:
	/*! MinusExpr constructor
	\param Expr for class Expr
	\param string for operator
	\param Expr for class Expr
	*/
	MinusExpr(Expr *l, std::string o, Expr *r):left(l),right(r),op(o){};
	virtual ~MinusExpr(){};
	/*! virtual unparse for MinusExpr */
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *left;
	Expr *right;
	std::string op;
};

//Expr ::= Expr '/' Expr
class DivExpr:public Expr
{
public:
	/*! DivExpr constructor
	\param Expr for class Expr
	\param string for operator
	\param Expr for class Expr
	*/
	DivExpr(Expr *l, std::string o, Expr *r):left(l),right(r),op(o){};
	virtual ~DivExpr(){};
	/*! virtual unparse for DivExpr*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *left;
	Expr *right;
	std::string op;
};

//Expr ::= Expr '%' Expr
class MouExpr:public Expr
{
public:
	/*! MouExpr constructor
	\param Expr for class Expr
	\param string for operator
	\param Expr for class Expr
	*/
	MouExpr(Expr *l, std::string o, Expr *r):left(l),right(r),op(o){};
	virtual ~MouExpr(){};
	/*! virtual unparse for MouExpr*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *left;
	Expr *right;
	std::string op;
};

// Expr ::= Expr '==' Expr  
// Expr ::= Expr '<' Expr
// Expr ::= Expr '>' Expr
// Expr ::= Expr '>=' Expr
// Expr ::= Expr '<=' Expr
// Expr ::= Expr '!=' Expr 

class BinOp:public Expr
{
public:
	/*! BinOp constructor
	\param Expr for class Expr
	\param string for operator
	\param Expr for class Expr
	*/
	BinOp(Expr *one, std::string o, Expr *two):d1(one),op(o),d2(two){};
	virtual ~BinOp(){};
	/*! virtual unparse for BinOp*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *d1;
	std::string op;
	Expr *d2;
	
};

// Expr ::= Expr ':' Expr
class ConsExpr:public Expr
{
public:
	/*! ConsExpr constructor
	\param Expr for class Expr
	\param string for operator
	\param Expr for class Expr
	*/
	ConsExpr(Expr *one,std::string o, Expr *two):d1(one),op(o),d2(two){};
	virtual ~ConsExpr(){};
	/*! virtual unparse for ConsExpr*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *d1;
	std::string op;
	Expr *d2;
};
// Expr ::= Expr '++' Expr 
class AppendExpr:public Expr
{
public:
	/*! AppendExpr constructor
	\param Expr for class Expr
	\param string for operator
	\param Expr for class Expr
	*/
	AppendExpr(Expr *one,std::string o,Expr *two):d1(one),op(o),d2(two){};
	virtual ~AppendExpr(){};
	/*! virtual unparse for AppendExpr*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *d1;
	std::string op;
	Expr *d2;
};
// Expr ::= 'let' DeclList 'in' Expr 'end'
class LetExpr:public Expr
{
public:
	/*! LetExpr constructor
	\param DeclList for class DeclList
	\param Expr for class Expr
	*/
	LetExpr(DeclList *one,Expr *two):d1(one),d2(two){};
	virtual ~LetExpr(){};
	/*! virtual unparse for LetExpr*/
	std::string unparse();
	std::string cppCode();
	/* data */

private:
	DeclList *d1;
	Expr *d2;
};

//Expr ::= 'if' Expr 'then' Expr 'else' Expr  
class IfThenElse:public Expr
{
public:
	/*! IfThenElseExpr constructor
	\param Expr for class Expr
	\param Expr for class Expr
	\param Expr for class Expr
	*/
	IfThenElse(Expr *i, Expr *t, Expr *e):ifexpr(i),thenexpr(t),elseexpr(e){};
	virtual ~IfThenElse(){};
	/*! virtual unparse for IfThenElse*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *ifexpr;
	Expr *thenexpr;
	Expr *elseexpr;
};

// Expr ::= 'print' '(' Expr ')' 
class PrintExpr:public Expr
{
public:
	/*! PrintExpr constructor
	\param Expr for class Expr
	\param string for operator
	*/	
	PrintExpr(Expr *one, std::string o):d1(one), op(o){};
	virtual ~PrintExpr(){};
	/*! virtual unparse for PrintExpr*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *d1;
	std::string op;
};

// Expr ::= 'write' '(' Expr, Expr ')'  
class WriteExpr:public Expr
{
public:
	/*! WriteExpr constructor
	\param Expr for class Expr
	\param string for operator
	\param Expr for class Expr
	*/	
	WriteExpr(Expr *one, std::string o, Expr *two):d1(one) ,op(o) ,d2(two) {};
	virtual ~WriteExpr(){};
	/*! virtual unparse for WriteExpr*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *d1;
	std::string op;
	Expr *d2;
	
};


// Expr ::= 'read' '<' TypeExpr '>' '(' Expr ')'
class ReadExpr:public Expr
{
public:
	/*! ReadExpr constructor
	\param TypeExpr for class TypeExpr
	\param string for read
	\param Expr for class Expr
	*/
	ReadExpr(TypeExpr *one, std::string o, Expr *two):d1(one),op(o),d2(two){};
	virtual ~ReadExpr(){};
	/*! virtual unparse for ReadExpr*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	TypeExpr *d1;
	std::string op;
	Expr *d2;
};

// Expr ::= 'head' '(' Expr ')'
class HeadExpr: public Expr
{
public:
	/*! HeadExpr constructor
	\param Expr for class Expr
	\param string for Head
	*/
	HeadExpr(Expr *one, std::string o): d1(one), op(o){};
	virtual ~HeadExpr(){};
	/*! virtual unparse for HeadExpr*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *d1;
	std::string op;
};

// Expr ::= 'tail' '(' Expr ')'
class TailExpr:public Expr
{
public:
	/*! TailExpr constructor
	\param Expr for class Expr
	\param string for Tail
	*/
	TailExpr(Expr *one, std::string o): d1(one), op(o){};
	virtual ~TailExpr(){};
	/*! virtual unparse for TailExpr*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *d1;
	std::string op;
};

// Expr ::= 'null' '(' Expr ')'
class NullExpr:public Expr
{
public:
	/*! NullExpr constructor
	\param Expr for class Expr
	\param string for Null
	*/
	NullExpr(Expr *one, std::string o):d1(one), op(o){};
	virtual ~NullExpr(){};
	/*! virtual unparse for NullExpr*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *d1;
	std::string op;
};

// Expr ::= 'map' '(' Expr ',' Expr ')'
class MapExpr: public Expr
{
public:
	/*! MapExpr constructor
	\param Expr for class Expr
	\param string for Map
	\param Expr for class Expr
	*/
	MapExpr(Expr *one, std::string o, Expr *two):d1(one), op(o), d2(two){};
	virtual ~MapExpr(){};
	/*! virtual unparse for MapExpr*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *d1;
	std::string op;
	Expr *d2;
};

// Expr ::= 'filter' '(' Expr ',' Expr ')'
class FilterExpr:public Expr
{
public:
	/*! FilterExpr constructor
	\param Expr for class Expr
	\param string for Filter
	\param Expr for class Expr
	*/
	FilterExpr(Expr *one, std::string o, Expr *two):d1(one),op(o),d2(two){};
	virtual ~FilterExpr(){};
	/*! virtual unparse for FilterExpr*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *d1;
	std::string op;
	Expr *d2;
};
// Expr ::= 'fold' '(' Expr ',' Expr ',' Expr ')'
class FoldExpr:public Expr
{
public:
	/*! FoldExpr constructor
	\param Expr for class Expr
	\param string for Fold
	\param Expr for class Expr
	\param Expr for class Expr
	*/
	FoldExpr(Expr *one, std::string o,Expr *two, Expr *three):d1(one),op(o),d2(two),d3(three){};
	virtual ~FoldExpr(){};
	/*! virtual unparse for FoldExpr*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	Expr *d1;
	std::string op;
	Expr *d2;
	Expr *d3;
};
// Expr ::= 'zip' '(' Expr ',' Expr ')'
class ZipExpr:public Expr
{
public:
	/*! ZipExpr constructor
	\param Expr for class Expr
	\param string for Zip
	\param Expr for class Expr
	*/
	ZipExpr(Expr *one, std::string o,Expr *two):d1(one), op(o), d2(two){};
	virtual ~ZipExpr(){};
	/*! virtual unparse for ZipExpr*/
	std::string unparse();
	std::string cppCode();

	/* data */
private:
	Expr *d1;
	std::string op;
	Expr *d2;
};


// Expr ::= '\' TypeBindingsList 'arrow' Expr
class LambdaExpr:public Expr
{
public:
	/*! LambdaExpr constructor
	\param TypeBindingList for class TypeBindingList
	\param string for arrow
	\param Expr for class Expr
	*/
	LambdaExpr(TypeBindingList *one, std::string o, Expr *two):d1(one),op(o),d2(two){};
	virtual ~LambdaExpr(){};
	/*! virtual unparse for LambdaExpr*/
	std::string unparse();
	std::string cppCode();
	/* data */
private:
	TypeBindingList *d1;
	std::string op;
	Expr *d2;
};

// Expr ::= tupleOp '(' Expr ')'
class ProjectionExpr:public Expr
{
public:
	/*! tupleOp constructor
	\param Expr for class Expr
	\param string for #tuple
	*/
	ProjectionExpr(Expr *one,std::string o): d1(one),op(o){};
	virtual ~ProjectionExpr(){};
	/*! virtual unparse for tupleOp*/
	std::string unparse(){return op+" ( "+d1->unparse()+" )";}
	std::string cppCode();
	/* data */
private:
	Expr *d1;
	//ExprList *d2;
	std::string op;
};


#endif // Node_H
