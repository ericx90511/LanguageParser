#include "AST.h"


#include <string>//
#include <iostream>//


using namespace std ;
// Root
// ------------------------------------------------------------


/* Program ::= DeclList */
string Program::unparse(){
	return d1->unparse();
}

string Program::cppCode(){
	return "#include \"../samples/common_header.h\" \n using namespace std;\n "+d1->cppCode();
}
/* DeclList ::= Decl
 DeclList ::= Decl DeclList */
string DeclList::unparse(){
	return d1->unparse()+op+d2->unparse();
}

string DeclList::cppCode(){
	return d1->cppCode()+d2->cppCode();
}

/* Decl ::= variableName '::' TypeExpr '=' Expr ';' */
string Decl::unparse(){
		return d1->unparse()+" :: "+d2->unparse()+" = "+d3->unparse()+" ;";
}

string Decl::cppCode(){
	TypeExprBinType *temp = dynamic_cast<TypeExprBinType*>(d2);
	if(temp){
		if(d1->cppCode()=="main"){
			return "int main ( int argc, char **argv ){\n MyList<string> args = mkArgs ( argc, argv) ; \n return "+d3->cppCode()+";\n"+"}";
		}
		else{
		return temp->getTypeExpr()->cppCode()+" "+d1->cppCode()+"( "+temp->getTypeBindingList()->cppCode()+" )"+"{\n"+"return "+d3->cppCode()+";\n"+"}";
		}
	}
	else{
	return d2->cppCode()+" "+d1->cppCode()+"="+d3->cppCode()+";\n";
    }
}

/* TypeExpr ::= 'Integer' 
   TypeExpr ::= 'Float' 
   TypeExpr ::= 'String' 
   TypeExpr ::= 'Boolean' */
string TypeExprKwd::unparse(){
	return op;
}

string TypeExprKwd::cppCode(){
	if (op=="Integer"){
		return "int";
	}
	else if(op == "Float"){
		return "float";
	}
	else if(op=="String"){
		return "string";
	}
	else if(op=="Boolean"){
		return "bool";
	}else{
		return "undefined";
	}
}
/*TypeExpr ::= '[' TypeExpr ']' */
string TypeExprBracket::unparse(){
	return "["+d1->unparse()+"]";
}

string TypeExprBracket::cppCode(){
	return "MyList<"+d1->cppCode()+">";
}

/*TypeExpr ::= '(' TypeBindingList ')' '->' TypeExpr */
string TypeExprBinType::unparse(){
	return "( "+d1->unparse()+" )"+" "+op+" "+d2->unparse();
}

string TypeExprBinType::cppCode(){
	return "TypeExprBinType undefined";
}

/*TypeExpr ::= '(' TypeExprList ')' */  
string TypeExprParen::unparse(){
	return "( "+d1->unparse()+" )";
}

string TypeExprParen::cppCode(){	
	return d1->cppCode();
}

// TypeExprList ::= TypeExpr ',' TypeExprList
string TypeExprList::unparse(){
	return d1->unparse()+op+d2->unparse();
}

string TypeExprList::cppCode(){
	//TypeExprList *tel = dynamic_cast<TypeExprList*>(d2);
	if(op == "") {
		return d1->cppCode();
	} else {
		return " pair<"+d1->cppCode()+op+d2->cppCode()+"> ";
	}
}

//TypeBindingList ::= TypeBinding
//TypeBindingList ::= TypeBinding ',' TypeBindingList

string TypeBindingList::unparse(){
	return d1->unparse()+op+d2->unparse();
}

string TypeBindingList::cppCode(){
	return d1->cppCode()+op+d2->cppCode();
}
//TypeBinding ::= variableName '::' TypeExpr
string TypeBinding::unparse(){
	return d1->unparse()+"::"+d2->unparse();
}

string TypeBinding::cppCode(){
	return d2->cppCode()+" "+d1->cppCode();
}

// OptionalExprList ::= <<empty>>
// OptionalExprList ::= ExprList
string OptionalExprList::unparse(){
	return d1->unparse();
}

string OptionalExprList::cppCode(){
	return d1->cppCode();
}

//ExprList ::= Expr
//ExprList ::= Expr ',' ExprList
string ExprList::unparse(){
	return d1->unparse()+op+d2->unparse();
}

string ExprList::cppCode(){
	return d1->cppCode()+op+d2->cppCode();
}

// Expr ::= variableName 

string VariableName::unparse(){
	return var;
}

string VariableName::cppCode(){
	return var;
}

// Expr ::= intConst 
string IntConst::unparse(){
	return intVar;
}

string IntConst::cppCode(){
	return intVar;
}

// Expr ::= floatConst 
string FloatConst::cppCode(){
	return intVar;
}
// Expr ::= stringConst 
// Expr ::= trueKwd 
string TrueKwd::cppCode(){
	return stringCon;
}
// Expr ::= falseKwd 
string FalseKwd::cppCode(){
	return stringCon;
}

// Expr ::= '(' Expr ')'  
// Expr ::= '(' Expr ',' ExprList ')' 
string NestedOrTupleExpr::unparse(){
	return "( "+d1->unparse()+op+" "+d2->unparse()+")";
}

string NestedOrTupleExpr::cppCode(){
	return "("+d1->cppCode()+op+d2->cppCode()+")";
}

// Expr ::= '[' '<' TypeExpr '>' OptionalExprList ']'  //after TypeExpr
string TypeOpt::unparse(){
	return "[ < "+d1->unparse()+" > "+d2->unparse()+"]";
}

string TypeOpt::cppCode(){
	string iter="";
	// ExprList *cur= d2->d1; 
	ExprList *cur = d2->getExprList();

	if(cur){
		while(!dynamic_cast<ExprEmpty*>(cur)){
		iter += "temp.push_back("+cur->getExpr()->cppCode()+");\n";
		cur=cur->getExprList();	
		}
	}
	else{	
		iter="";
	}
	return "({MyList<"+d1->cppCode()+"> temp; \n"+" "+iter+" temp;})";
}

// Expr ::=  '[' Expr '..' Expr ']'   
string TwoExpr::unparse(){

	return "[ "+d1->unparse()+" .. "+d2->unparse()+" ]";
}

string TwoExpr::cppCode(){
	return "({ MyList<int> temp("+d1->cppCode()+","+ d2->cppCode()+"); temp; })";
}

// Expr ::= variableName '(' OptionnalExprList ')'
string FunctionCall::unparse(){
	return d2->unparse()+" ( "+d1->unparse()+")";
}
string FunctionCall::cppCode(){
	return "("+d2->cppCode()+"("+d1->cppCode()+")"+")";
}

//Expr ::= Expr '+' Expr 
string PlusExpr::unparse(){
	return left->unparse()+" "+op+" "+right->unparse();
}

string PlusExpr::cppCode(){
	return "("+left->cppCode()+" "+op+" "+right->cppCode()+")";
}

//Expr ::= Expr '*' Expr 
string MulExpr::unparse(){
	return left->unparse()+" "+op+" "+right->unparse();
}


string MulExpr::cppCode(){
	return "("+left->cppCode()+" "+op+" "+right->cppCode()+")";
}

//Expr ::= Expr '-' Expr 
string MinusExpr::unparse(){
	return left->unparse()+" "+op+" "+right->unparse();
}

string MinusExpr::cppCode(){
	return left->cppCode()+" "+op+" "+right->cppCode();
}

//Expr ::= Expr '/' Expr  
string DivExpr::unparse(){
	return left->unparse()+" "+op+" "+right->unparse();
}

string DivExpr::cppCode(){
	return left->cppCode()+" "+op+" "+right->cppCode();
}

//Expr ::= Expr '%' Expr 
string MouExpr::unparse(){
	return left->unparse()+" "+op+" "+right->unparse();
}

string MouExpr::cppCode(){
	return left->cppCode()+" "+op+" "+right->cppCode();
}

// Expr ::= Expr '==' Expr  
// Expr ::= Expr '<' Expr
// Expr ::= Expr '>' Expr
// Expr ::= Expr '>=' Expr
// Expr ::= Expr '<=' Expr
// Expr ::= Expr '!=' Expr
string BinOp::unparse(){
	return d1->unparse()+" "+op+" "+d2->unparse();
}

string BinOp::cppCode(){
	return d1->cppCode()+" "+op+" "+d2->cppCode();
}

// Expr ::= Expr ':' Expr  
string ConsExpr::unparse(){
	return d1->unparse()+" "+op+" "+d2->unparse();
}  

string ConsExpr::cppCode(){
	/* order of d1 and d2 switch */
	return "("+d2->cppCode()+".cons"+d1->cppCode()+")";
}

// Expr ::= Expr '++' Expr
string AppendExpr::unparse(){
	return d1->unparse()+" "+op+" "+d2->unparse();
}

string AppendExpr::cppCode(){
	return d1->cppCode()+" "+op+" "+d2->cppCode();
}

//Expr ::= 'let' DeclList 'in' Expr 'end'  
string LetExpr::unparse(){
	return "let "+d1->unparse()+" in "+d2->unparse()+" end";
}

string LetExpr::cppCode(){
	return "({"+d1->cppCode()+"\n"+d2->cppCode()+"; })";
}

//Expr ::= 'if' Expr 'then' Expr 'else' Expr 
string IfThenElse::unparse(){
	return "if "+ifexpr->unparse()+" then "+thenexpr->unparse()+" else "+elseexpr->unparse();
}

string IfThenElse::cppCode(){
	return "( "+ifexpr->cppCode()+" ? "+thenexpr->cppCode()+" : "+elseexpr->cppCode()+")";
}

//Expr ::= 'print' '(' Expr ')'  
string PrintExpr::unparse(){
	return op+" ( "+d1->unparse()+" )";
}

string PrintExpr::cppCode(){
	return "({ cout << "+d1->cppCode()+" << endl ; cout.good() ? 0 : 1 ; })  ";
}

//Expr ::= 'write' '(' Expr, Expr ')' 
string WriteExpr::unparse(){
	return op+" ( "+d1->unparse()+", "+d2->unparse()+" )";
}

string WriteExpr::cppCode(){
	return "WriteExpr undefined";
}

// Expr ::= 'read' '<' TypeExpr '>' '(' Expr ')'
string ReadExpr::unparse(){
	return "read < "+d1->unparse()+" >"+" ( "+d2->unparse()+" )";
}

string ReadExpr::cppCode(){
	return "({ "+d1->cppCode()+" temp ;  ifstream inStream("+d2->cppCode()+") ; inStream >> temp ; temp ; })";
}

// Expr ::= 'head' '(' Expr ')'
string HeadExpr::unparse(){
	return "head ( " + d1->unparse()+ " )";
}

string HeadExpr::cppCode(){
	return "("+d1->cppCode()+".head()"+")";
}

//Expr ::= 'tail' '(' Expr ')'
string TailExpr::unparse(){
	return op+" ( "+d1->unparse()+" )";
}

string TailExpr::cppCode(){
	return "("+d1->cppCode()+".tail()"+")";
}

//Expr ::= 'null' '(' Expr ')'
string NullExpr::unparse(){
	return op+" ( "+d1->unparse()+" )";
}

string NullExpr::cppCode(){
	return "("+d1->cppCode()+".null()"+")";
}

//Expr ::= 'map' '(' Expr ',' Expr ')'
string MapExpr::unparse(){
	return op+" ( "+d1->unparse()+", "+d2->unparse()+" )";
}

string MapExpr::cppCode(){
	return d2->cppCode()+".map("+d1->cppCode()+")";
}

//Expr ::= 'filter' '(' Expr ',' Expr ')'
string FilterExpr::unparse(){
	return op+" ( "+d1->unparse()+", "+d2->unparse()+" )";
}

string FilterExpr::cppCode(){
	return "("+d2->cppCode()+".filter("+d1->cppCode()+") )";
}

//Expr ::= 'fold' '(' Expr ',' Expr ',' Expr ')'
string FoldExpr::unparse(){
	return op+" ( "+d1->unparse()+", "+d2->unparse()+", "+d3->unparse()+" )";
}

string FoldExpr::cppCode(){
	return "("+d3->cppCode()+".fold("+d1->cppCode()+","+d2->cppCode()+") )";
}

//Expr ::= 'zip' '(' Expr ',' Expr ')'
string ZipExpr::unparse(){
	return op+" ( "+d1->unparse()+", "+d2->unparse()+" )";
}

string ZipExpr::cppCode(){
	return "("+d1->cppCode()+".zip("+d2->cppCode()+") )";
}

//Expr ::= '\' TypeBindingList '->' Expr
string LambdaExpr::unparse(){
	return "\\ "+d1->unparse()+" "+op+" "+d2->unparse();
}

string LambdaExpr::cppCode(){
	return "LambdaExpr undefined";
}

// Expr ::= tupleOp '(' Expr ')'
string ProjectionExpr::cppCode(){
	return "ProjectionExpr undefined";
}